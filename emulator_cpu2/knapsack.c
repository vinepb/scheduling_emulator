/**
 * @file knapsack.c
 * @author Vinicius Pimenta Bernardo (viniciuspibi@gmail.com)
 * @brief Dynamic programming solution for the knapsack problem.
 * @version 0.1
 * @date 2022-01-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>
#include <stdlib.h>

#include "tasks.h"
#include "knapsack.h"

/**
 * @brief Calculates the maximum between two numbers.
 * 
 * @param[in] a First number.
 * @param[in] b Second number.
 * @return @c uint32_t The greater number.
 */
static uint32_t max(uint32_t a, uint32_t b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

uint32_t dynamic_knapsack(uint32_t max_weight)
{
    uint32_t aux_matrix[NUMBER_OF_ITEMS + 1][SUM_OF_WEIGHTS + 1];
    uint32_t best = 0;
    uint32_t items = 0, k = 0;
    int i, j;

    // Cap max_weight to avoid overflow
    if (max_weight > SUM_OF_WEIGHTS)
    {
        max_weight = SUM_OF_WEIGHTS;
    }

    // Initialize all values of the matrix to 0.
    for (i = 0; i < (NUMBER_OF_ITEMS + 1); i ++)
    {
        for (j = 0; j < (max_weight + 1); j++)
        {
            aux_matrix[i][j] = 0;
        }
    }

    // Fill the matrix with the values of the knapsack problem.
    for (i = 0; i < (NUMBER_OF_ITEMS + 1); i++)
    {
        for (j = 0; j < (max_weight + 1); j++)
        {
            if (i == 0 || j == 0)
            {
                aux_matrix[i][j] = 0;
            }
            else if (weights[i - 1] <= j)
            {
                aux_matrix[i][j] = max(values[i - 1] + aux_matrix[i - 1][j - weights[i - 1]], aux_matrix[i - 1][j]);
            }
            else{
                aux_matrix[i][j] = aux_matrix[i - 1][j];
            }
        }
    }

    best = aux_matrix[NUMBER_OF_ITEMS][max_weight];
    k = max_weight;
    items = 0;

    // Find the items that will be in the knapsack.
    for (i = NUMBER_OF_ITEMS; i > 0; i--)
    {
        if (aux_matrix[i - 1][k] != best)
        {
            items |= (1 << (i - 1));
            k = (k - weights[i - 1]);
            best = aux_matrix[i][k];
        }
    }

    // Free the memory used by the matrix.
    for (i = 0; i < (NUMBER_OF_ITEMS + 1); i++)
    {
        free(aux_matrix[i]);
    }
    free(aux_matrix);

    return items;
    // return best;
}
