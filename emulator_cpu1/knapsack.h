/**
 * @file knapsack.h
 * @brief Dynamic knapsack algorithm declaration.
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef KNAPSACK_H
#define KNAPSACK_H

#define TASK_COUNT 4

/**
 * @brief Dynamic priority allocation.
 * 
 */
void dynamic_priority(void);

/**
 * @brief Dynamic knapsack algorithm implementation.
 * 
 * @param[in] max_power_consumption Maximum power consumption (aka max weigth).
 * @return @c uint16_t Maximum value achieved by the algorithm.
 */
uint16_t dynamic_knapsack(uint16_t max_power_consumption);

#endif // KNAPSACK_H