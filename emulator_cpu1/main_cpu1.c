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

#define IPC_CMD_READ_DATA   0x1001

/**
 * @brief CPU1 main function.
 */
void main(void)
{
    uint32_t rand;
    uint32_t data;

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

    while(1)
    {
        rand = rand_generator();

        printf("CPU1: Sending data: %ld\n", rand);

        // Send a message without message queue
        IPC_sendCommand(IPC_CPU1_L_CPU2_R, IPC_FLAG0, IPC_ADDR_CORRECTION_ENABLE,
        IPC_CMD_READ_DATA, 0, rand);

        // Wait for acknowledgment
        IPC_waitForAck(IPC_CPU1_L_CPU2_R, IPC_FLAG0);

        // Read response
        data = IPC_getResponse(IPC_CPU1_L_CPU2_R);

        printf("CPU1: Received data: %ld\n", data);

    }
}
