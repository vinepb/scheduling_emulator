/**
 * @file main_cpu1.c
 * @author Vinicius Pimenta Bernardo (viniciuspibi@gmail.com)
 * @brief This file contains the main function for CPU1.
 * @version 0.1
 * @date 2022-01-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//############################################################################
// $Copyright:
// Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//############################################################################

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "driverlib.h"
#include "device.h"
#include "ipc.h"

#include "rand_generator.h"

uint32_t GPIO_pins[] = {104U, 105U, 22U};

/**
 * @brief Initialize ADC A.
 */
void init_ADC_A(void);

/**
 * @brief Initialize ADC A start-of-conversion (SOC).
 */
void init_ADC_A_SOC(void);

/**
 * @brief CPU1 main function.
 */
void main(void)
{
    uint32_t data;
    uint16_t ADCA_Result;
    int i;

    // Configure system clock and PLL, enable peripherals, and configure
    // flash if used.
    Device_init();

#ifdef _STANDALONE
#ifdef _FLASH
    // TODO check to see if this breaks.
    Device_bootCPU2(BOOTMODE_BOOT_TO_FLASH_SECTOR0);
#else
    // TODO this breaks the RAM build.
    Device_bootCPU2(BOOTMODE_BOOT_TO_M0RAM);
#endif
#endif

    // Initialize GPIO and configure the GPIO pin as a push-pull output
    Device_initGPIO();
    for (i = 0; i < (sizeof(GPIO_pins)/sizeof(uint32_t)); i++)
    {
        GPIO_setPadConfig(GPIO_pins[i], GPIO_PIN_TYPE_STD);
        GPIO_setDirectionMode(GPIO_pins[i], GPIO_DIR_MODE_OUT);
    }

    // Initialize the PIE module and vector table.
    Interrupt_initModule();
    Interrupt_initVectorTable();

    // Clear any IPC flags if set already
    IPC_clearFlagLtoR(IPC_CPU1_L_CPU2_R, IPC_FLAG_ALL);

    // Synchronize both the cores.
    IPC_sync(IPC_CPU1_L_CPU2_R, IPC_FLAG17);

    init_ADC_A();
    init_ADC_A_SOC();

    // Enable global interrupts.
    EINT;
    // Enable real-time debug.
    ERTM;

    while(1)
    {
        data = rand_generator();

        // Convert, wait for completion, and store results
        ADC_forceMultipleSOC(ADCA_BASE, ADC_FORCE_SOC0);

        // Wait for ADCA to complete, then acknowledge flag
        while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false);
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
        
        // Store results
        ADCA_Result = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

        data = (((uint32_t)ADCA_Result) * 100) / 4095;

        printf("CPU1: Sending data: %ld\n", data);

        // Send a message without message queue
        IPC_sendCommand(IPC_CPU1_L_CPU2_R, IPC_FLAG0, IPC_ADDR_CORRECTION_ENABLE,
        0, 0, data);

        // Wait for acknowledgment
        IPC_waitForAck(IPC_CPU1_L_CPU2_R, IPC_FLAG0);

        // Read response
        data = IPC_getResponse(IPC_CPU1_L_CPU2_R);

        for (i = 0; i < (sizeof(GPIO_pins)/sizeof(uint32_t)); i++)
        {
            if ((data >> i) & 1UL)
            {
                GPIO_writePin(GPIO_pins[i], 1);
            }
            else
            {
                GPIO_writePin(GPIO_pins[i], 0);
            }
        }

        printf("CPU1: Received data: %ld\n", data);
    }
}

void init_ADC_A(void)
{
    // Set ADCDLK divider to /4
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);

    // Set resolution and signal mode and load
    // corresponding trims.
    ADC_setMode(ADCA_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);

    // Set pulse positions to late
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);

    // Power up the ADC and delay for 1 ms
    ADC_enableConverter(ADCA_BASE);

    DEVICE_DELAY_US(1000);
}

void init_ADC_A_SOC(void)
{   
    // Configure SOC0 to convert pin A0 by software only
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY,
                 ADC_CH_ADCIN0, 15);

    // Set SOC0 to set interrupt 1 flag. Enable the interrupt and make
    // sure irs flag is cleared.
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
}
