/**
 * @file knapsack.h
 * @author Vinicius Pimenta Bernardo (viniciuspibi@gmail.com)
 * @brief Dynamic programming solution for the knapsack problem.
 * @version 0.1
 * @date 2022-01-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef KNAPSACK_H_
#define KNAPSACK_H_

#include <stdint.h>

#define LINEAR_PRIORITY 1
#define EXPONENTIAL_PRIORITY 2

/**
 * @brief Dynamic programming solution for the knapsack problem.
 * 
 * @param[in] max_weight Maximum weight of the knapsack.
 * @return @c uint32_t Items that can be put in the knapsack.
 */
uint32_t dynamic_knapsack(uint32_t max_weight);

/**
 * @brief Dynamically change tasks priority.
 * 
 * @param[in] items Current items in the knapsack.
 * @return @c uint32_t Deadline loss counter.
 */
uint32_t dynamic_priority(uint32_t items);

/**
 * @brief Simulate the mppt algorithm.
 *
 * @param[in] PspTotalNew Total solar panel power.
 * @param[in] PloadNew Current load consumption.
 * @return @c uint32_t Available power.
 */
uint32_t perturb_observe(uint32_t PspTotalNew, uint32_t PloadNew);

#endif /* KNAPSACK_H_ */
