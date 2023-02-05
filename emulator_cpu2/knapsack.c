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

#define PRIORITY_TYPE EXPONENTIAL_PRIORITY
#ifndef PRIORITY_TYPE
#define PRIORITY_TYPE LINEAR_PRIORITY
#endif

#if PRIORITY_TYPE == EXPONENTIAL_PRIORITY
static const uint32_t priority_exp_lut[] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 13, 13, 14, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28, 29, 30, 32, 33, 35, 36, 38, 40, 42, 44, 46, 48, 50, 52, 55, 58, 60, 63, 66, 69, 72, 76, 79, 83, 87, 91, 95, 100};
#endif

uint32_t aux_matrix[TASK_COUNT + 1][SUM_OF_WEIGHTS + 1] = {0};

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
    uint32_t best = 0;
    uint32_t items = 0, k = 0;
    int j, i;

    // Cap max_weight to avoid overflow
    if (max_weight > SUM_OF_WEIGHTS)
    {
        max_weight = SUM_OF_WEIGHTS;
    }

    // Initialize all values of the matrix to 0.
    for (j = 0; j < (TASK_COUNT + 1); j++)
    {
        for (i = 0; i < (max_weight + 1); i++)
        {
            aux_matrix[j][i] = 0;
        }
    }

    // Fill the matrix with the values of the knapsack problem.
    for (j = 0; j < (TASK_COUNT + 1); j++)
    {
        for (i = 0; i < (max_weight + 1); i++)
        {
            if (j == 0 || i == 0)
            {
                aux_matrix[j][i] = 0;
            }
            else if (weights[j - 1] <= i)
            {
                aux_matrix[j][i] = max(values[j - 1] + aux_matrix[j - 1][i - weights[j - 1]], aux_matrix[j - 1][i]);
            }
            else
            {
                aux_matrix[j][i] = aux_matrix[j - 1][i];
            }
        }
    }

    best = aux_matrix[TASK_COUNT][max_weight];
    k = max_weight;
    items = 0;

    // Find the items that will be in the knapsack.
    for (j = TASK_COUNT; j > 0; j--)
    {
        if (aux_matrix[j - 1][k] != best)
        {
            items |= (1 << (j - 1));
            k = (k - weights[j - 1]);
            best = aux_matrix[j][k];
        }
    }

    return items;
    // return best;
}

uint32_t dynamic_priority(uint32_t items)
{
    static uint32_t tick = 0;
    static uint32_t deadline_loss_counter = 0;
    int i;

    for (i = 0; i < TASK_COUNT; i++)
    {
        if (items & (1UL << i))
        {
            task_on_off_status[i] = true;
        }
        else
        {
            task_on_off_status[i] = false;
        }
    }

    for (i = 0; i < TASK_COUNT; i++)
    {
        // Did the task finish?
        if (task_computing_time[i] - task_executed_time[i] <= 0)
        {
            // Variable initialization.
            task_on_off_status[i] = false;
            task_executed_time[i] = 0;
            values[i] = 1;
            task_already_executed[i] = true;
        }
        // Check for deadline loss.
        if ((tick > 0) && (tick == task_deadline[i]) && (task_already_executed[i] == false))
        {
            deadline_loss_counter++;
        }
        // New deadline calculation.
        if (tick == task_deadline[i])
        {
            task_deadline[i] += task_period[i];
            task_already_executed[i] = 0;
        }
        // Is task running?
        if (task_on_off_status[i] == true)
        {
            task_executed_time[i] += 1;
        }
        // Next deadline loss.
        if ((task_deadline[i] - task_computing_time[i] - tick) <= 0)
        {
            values[i] = UINT16_MAX;
        }
        else
        {
// Priority calculation.
#if PRIORITY_TYPE == LINEAR_PRIORITY
            values[i] = max(1, (100UL - ((100UL * (task_deadline[i] - tick)) / task_period[i])));
#elif PRIORITY_TYPE == EXPONENTIAL_PRIORITY
            values[i] = priority_exp_lut[max(1, (100UL - ((100UL * (task_deadline[i] - tick)) / task_period[i]))) - 1];
#endif
        }
        // If the task has already been executed.
        if (task_already_executed[i] == true)
        {
            values[i] = 1;
        }
    }

    tick += 1;

    return deadline_loss_counter;
}

uint32_t perturb_observe(uint32_t PspTotalNew, uint32_t Win)
{
    static uint32_t PspTotal = 0UL, PspTotalOld = 0UL;
    static uint32_t perturb = 0UL, perturbOld = 0UL;
    static const uint32_t step = 1UL;
    uint32_t Wout = 0UL;

    PspTotalOld = PspTotal;
    PspTotal = PspTotalNew;
    perturbOld = perturb;

    if (PspTotalOld > PspTotal)
    {
        (perturbOld == 0) ? (perturb = 1) : (perturb = 0);
    }
    else
    {
        perturb = perturbOld;
    }

    (perturb) ? (Wout = Win - step) : (Wout = Win + step);
    if (Wout <= 10UL)
    {
        Wout = 10UL;
    }

    return Wout;
}
