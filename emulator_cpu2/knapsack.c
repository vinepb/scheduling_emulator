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

#include "knapsack.h"

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a > _b ? _a : _b; })

uint32_t dynamic_knapsack(uint32_t max_weight, uint32_t number_of_items, uint32_t *vector_value, uint32_t *vector_weight)
{
    uint32_t **aux_matrix, i, j;
    // The aux_matrix is a matrix that will be used to store the results of the
    // dynamic programming.
    // The first row and column of the matrix are not used.
    aux_matrix = (uint32_t **) malloc(number_of_items * sizeof(uint32_t *));
    for (i = 0; i < (number_of_items + 1); i++)
    {
        aux_matrix[i] = (uint32_t *) malloc((max_weight + 1) * sizeof(uint32_t));
    }

    // Initialize all values of the matrix to 0.
    for (i = 0; i < (number_of_items + 1); i ++)
    {
        for (j = 0; j < (max_weight + 1); j++)
        {
            aux_matrix[i][j] = 0;
        }
    }

    // Fill the matrix with the values of the knapsack problem.
    for (i = 0; i < (number_of_items + 1); i++)
    {
        for (j = 0; j < (max_weight + 1); j++)
        {
            if (i == 0 || j == 0)
            {
                aux_matrix[i][j] = 0;
            }
            else if (vector_weight[i - 1] <= j)
            {
                aux_matrix[i][j] = max(vector_value[i - 1] + aux_matrix[i - 1][j - vector_weight[i - 1]], aux_matrix[i - 1][j]);
            }
            else{
                aux_matrix[i][j] = aux_matrix[i - 1][j];
            }
        }
    }

    uint32_t best = aux_matrix[number_of_items][max_weight];

    return best;
}
