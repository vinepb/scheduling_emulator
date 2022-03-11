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
#include <string.h>

#include "driverlib.h"
#include "device.h"
#include "ipc.h"

#include "rand_generator.h"

// User config defines *********************
#define USE_TIMER 0
#define USE_ADC 1

#define TIMER0_PERIOD_MS 100
#define SCIA_BAURATE 9600

#define TASK_COUNT 3
//******************************************
// Don't change this section************ ***
#if !USE_TIMER
#ifdef USE_ADC
#undef USE_ADC
#define USE_ADC USE_TIMER
#endif
#endif
//******************************************

// LEDs pins.
uint32_t GPIO_pins[] = {104U, 105U, 22U};

#if USE_TIMER
/**
 * @brief Timer 0 Interrupt Service Routine.
 */
__interrupt void cpuTimer0ISR(void);

/**
 * @brief Initialize timer 0
 * 
 * @param[in] freq System clock frequency
 * @param[in] period Timer period
 */
void initCPUTimer0(float freq, float period);

#if USE_ADC
/**
 * @brief Initialize ADC A.
 */
void initADCA(void);

/**
 * @brief Initialize ADC A start-of-conversion (SOC).
 */
void initADCASOC(void);
#endif

#else

/**
 * @brief Initialize Serial Communication Interface A.
 */
void initSCIA(void);

#endif

/**
 * @brief CPU1 main function.
 */
void main(void)
{
    int j;
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

    // Initialize GPIO and configure the GPIO pin as a push-pull output.
    Device_initGPIO();

#if USE_TIMER
    int i;
    for (i = 0; i < (sizeof(GPIO_pins)/sizeof(uint32_t)); i++)
    {
        GPIO_setPadConfig(GPIO_pins[i], GPIO_PIN_TYPE_STD);
        GPIO_setDirectionMode(GPIO_pins[i], GPIO_DIR_MODE_OUT);
    }
#else
    // Configuration for the SCI Rx pin.
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCIRXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC);

    // Configuration for the SCI Tx pin.
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCITXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC);
#endif

    // Initialize the PIE module and vector table.
    Interrupt_initModule();
    Interrupt_initVectorTable();

    // Clear any IPC flags if set already
    IPC_clearFlagLtoR(IPC_CPU1_L_CPU2_R, IPC_FLAG_ALL);

    // Synchronize both the cores.
    IPC_sync(IPC_CPU1_L_CPU2_R, IPC_FLAG17);

    // Enable global interrupts.
    EINT;
    // Enable real-time debug.
    ERTM;

#if USE_TIMER
#if USE_ADC
    initADCA();
    initADCASOC();
#endif
    initCPUTimer0(DEVICE_SYSCLK_FREQ, TIMER0_PERIOD_MS * 1000UL);
#else
    initSCIA();
    uint32_t uartData[2];
    uint16_t rxStatus = 0U;
#endif
    uint32_t IPCresponse;

    while(1)
    {
#if !USE_TIMER
        for (j = 0; j < 2; j++)
        {
            // Read a character from the FIFO.
            uartData[j] = (uint32_t) SCI_readCharBlockingFIFO(SCIA_BASE);

            rxStatus = SCI_getRxStatus(SCIA_BASE);
            if((rxStatus & SCI_RXSTATUS_ERROR) != 0)
            {
                //If Execution stops here there is some error
                //Analyze SCI_getRxStatus() API return value
                ESTOP0;
            }
        }

        printf("CPU1: Sending data: %ld %ld\n", uartData[0], uartData[1]);

        // Send a message without message queue
        IPC_sendCommand(IPC_CPU1_L_CPU2_R, IPC_FLAG0, IPC_ADDR_CORRECTION_ENABLE,
        0, uartData[0], uartData[1]);

        // Wait for acknowledgment
        IPC_waitForAck(IPC_CPU1_L_CPU2_R, IPC_FLAG0);

        // Read response
        IPCresponse = IPC_getResponse(IPC_CPU1_L_CPU2_R);

        printf("CPU1: Received data: %ld\n", IPCresponse);

        // Clear CPU interrupt flag
        Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

        for (j = 0; j < TASK_COUNT; j++)
        {
            // For each bit in uartData, send '1' or '0' via SCI
            ((IPCresponse >> j) & 1UL) ? SCI_writeCharBlockingFIFO(SCIA_BASE, '1') : SCI_writeCharBlockingFIFO(SCIA_BASE, '0');
        }
#endif
    }
}

#if USE_TIMER
__interrupt void cpuTimer0ISR(void)
{
    uint32_t data;
    int i;

#if USE_ADC
    uint16_t ADCA_Result;
    // Convert, wait for completion, and store results
    ADC_forceMultipleSOC(ADCA_BASE, ADC_FORCE_SOC0);

    // Wait for ADCA to complete, then acknowledge flag
    while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    
    // Store results
    ADCA_Result = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

    data = (((uint32_t)ADCA_Result) * 100) / 4095;
#else /* Use random number generator instead */
    data = rand_generator();
#endif /* USE_ADC */

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

    // Clear CPU interrupt flag
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

void initCPUTimer0(float freq, float period)
{
    uint32_t temp;
    // Initialize timer period
    temp = (uint32_t)(freq / 1000000 * period);
    CPUTimer_setPeriod(CPUTIMER0_BASE, temp);

    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);

    // Make sure timer is stopped
    CPUTimer_stopTimer(CPUTIMER0_BASE);

    // Reload counter register with period value
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);

    // Register interrupt handler
    Interrupt_register(INT_TIMER0, &cpuTimer0ISR);

    // Disable free run
    CPUTimer_setEmulationMode(CPUTIMER0_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);

    // Enable timer interrupt
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);

    // Enable CPU int1, which is connected to CPU-Timer 0
    Interrupt_enable(INT_TIMER0);

    // Start timer
    CPUTimer_startTimer(CPUTIMER0_BASE);
}

#if USE_ADC
void initADCA(void)
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

void initADCASOC(void)
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
#endif

#else

void initSCIA()
{
    SCI_performSoftwareReset(SCIA_BASE);

    // 8 char bits, 1 stop bit, no parity.
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, SCIA_BAURATE, (SCI_CONFIG_WLEN_8 |
                                                                SCI_CONFIG_STOP_ONE |
                                                                SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIA_BASE);
    SCI_resetRxFIFO(SCIA_BASE);
    SCI_resetTxFIFO(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(SCIA_BASE);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);

#ifdef AUTOBAUD
    // Perform an autobaud lock.
    // SCI expects an 'a' or 'A' to lock the baud rate.
    SCI_lockAutobaud(SCIA_BASE);
#endif
}

#endif
