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

/**
 * @brief Dynamic programming solution for the knapsack problem.
 * 
 * @param[in] max_weight Maximum weight of the knapsack. 
 * @param[in] number_of_items Number of items.
 * @param[in] vector_value Vector with the value of each item.
 * @param[in] vector_weight Vector with the weight of each item.
 * @return @c uint32_t Items that can be put in the knapsack.
 */
uint32_t dynamic_knapsack(uint32_t max_weight, uint32_t number_of_items, uint32_t *vector_value, uint32_t *vector_weight);

#endif /* KNAPSACK_H_ */
