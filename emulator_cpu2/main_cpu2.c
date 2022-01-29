/**
 * @file main_cpu2.c
 * @author Vinicius Pimenta Bernardo (viniciuspibi@gmail.com)
 * @brief This file contains the main function for CPU2.
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

#include "knapsack.h"

#define IPC_CMD_READ_DATA   0x1001

uint32_t number_of_items = 3;
uint32_t values[3] = {100,50,150};
uint32_t weights[3] = {20,10,30};

/**
 * @brief IPC 0 Interrupt Service Routine. 
 */
__interrupt void ipc0_ISR();

/**
 * @brief CPU2 main function.
 */
void main(void)
{
    // Configure system clock and PLL, enable peripherals, and configure
    // flash if used.
    Device_init();

    // Initialize the PIE module and vector table.
    Interrupt_initModule();
    Interrupt_initVectorTable();

    // Clear any IPC flags if set already
    IPC_clearFlagLtoR(IPC_CPU2_L_CPU1_R, IPC_FLAG_ALL);

    // Enable IPC0 interrupt.
    IPC_registerInterrupt(IPC_CPU2_L_CPU1_R, IPC_INT0, ipc0_ISR);

    // Enable global interrupts.
    EINT;
    // Enable real-time debug.
    ERTM;

    // Synchronize both the cores.
    IPC_sync(IPC_CPU2_L_CPU1_R, IPC_FLAG17);

    while(1)
    {

    }
}

__interrupt void ipc0_ISR(void)
{
    uint32_t cmd, addr, data;

    // Read the data from the IPC registers.
    IPC_readCommand(IPC_CPU2_L_CPU1_R, IPC_FLAG0, false, &cmd, &addr, &data);

    printf("CPU2: Received command: %ld data: %ld\n", cmd, data);

    data = dynamic_knapsack(data, number_of_items, values, weights);

    printf("CPU2: Sending data: %ld\n", data);

    // Send response.
    IPC_sendResponse(IPC_CPU2_L_CPU1_R, data);

    // Acknowledge IPC0 flag from remote.
    IPC_ackFlagRtoL(IPC_CPU2_L_CPU1_R, IPC_FLAG0);

    // Clear interrupt flags.
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    
}
