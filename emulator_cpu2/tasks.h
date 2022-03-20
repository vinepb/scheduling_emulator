/**
 * @file tasks.h
 * @author Vinicius Pimenta Bernardo (viniciuspibi@gmail.com)
 * @brief This file contains the tasks definitions.
 * @version 0.1
 * @date 2022-02-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TASKS_H_
#define TASKS_H_

#include <stdint.h>
#include <stdbool.h>

/* Test case 00 ************************************************************/
#define TASK_COUNT 4
#define SUM_OF_WEIGHTS 100
const uint32_t      weights[TASK_COUNT] = {20, 10, 30, 40};
const uint32_t      task_period[TASK_COUNT] = {20, 30, 60, 80};
const uint32_t      task_computing_time[TASK_COUNT] = {10, 20, 30, 40};
/***************************************************************************/

/* Test case 01 ************************************************************/
// #define TASK_COUNT 31
// #define SUM_OF_WEIGHTS 94
// const uint32_t      weights[TASK_COUNT] = {1, 7, 7, 5, 2, 1, 2, 1, 3, 4, 8, 6, 1, 1, 3, 1, 1, 4, 5, 4, 1, 1, 1, 4, 3, 7, 4, 1, 1, 3, 1};
// const uint32_t      task_period[TASK_COUNT] = {350, 350, 50, 50, 50, 10, 10, 10, 10, 4320, 5400, 1200, 10, 5, 20, 10, 10, 10, 10, 10, 10, 20, 5, 5, 10, 4320, 5, 10, 10, 4320, 120};
// const uint32_t      task_computing_time[TASK_COUNT] = {20, 20, 10, 2, 2, 1, 2, 1, 1, 5, 120, 50, 1, 1, 5, 1, 1, 1, 2, 1, 1, 1, 2, 2, 3, 2, 1, 2, 1, 5, 2};
/***************************************************************************/

volatile uint32_t   values[TASK_COUNT];
volatile bool       task_on_off_status[TASK_COUNT];
volatile uint32_t   task_deadline[TASK_COUNT];
volatile uint32_t   task_executed_time[TASK_COUNT];
volatile bool       task_already_executed[TASK_COUNT];

#endif /* TASKS_H_ */
