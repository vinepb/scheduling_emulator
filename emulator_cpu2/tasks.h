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

#define TASK_COUNT 7
#define SUM_OF_WEIGHTS 78

const uint32_t      weights[TASK_COUNT] = {13, 12, 8, 14, 14, 7, 10};
const uint32_t      task_period[TASK_COUNT] = {78, 438, 545, 454, 239, 65, 38};
const uint32_t      task_computing_time[TASK_COUNT] = {36, 113, 324, 136, 166, 24, 31};

volatile uint32_t   values[TASK_COUNT];
volatile bool       task_on_off_status[TASK_COUNT];
volatile uint32_t   task_deadline[TASK_COUNT];
volatile uint32_t   task_executed_time[TASK_COUNT];
volatile bool       task_already_executed[TASK_COUNT];

#endif /* TASKS_H_ */
