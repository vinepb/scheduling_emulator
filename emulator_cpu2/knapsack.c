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
    if (a > b) { return a;}
    else { return b;}
}

uint32_t dynamic_knapsack(uint32_t max_weight)
{
    uint32_t best = 0;
    uint32_t items = 0, k = 0;
    int i, j;

    // Cap max_weight to avoid overflow
    if (max_weight > SUM_OF_WEIGHTS)
    {
        max_weight = SUM_OF_WEIGHTS;
    }

    // Initialize all values of the matrix to 0.
    for (i = 0; i < (TASK_COUNT + 1); i ++)
    {
        for (j = 0; j < (max_weight + 1); j++)
        {
            aux_matrix[i][j] = 0;
        }
    }

    // Fill the matrix with the values of the knapsack problem.
    for (i = 0; i < (TASK_COUNT + 1); i++)
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

    best = aux_matrix[TASK_COUNT][max_weight];
    k = max_weight;
    items = 0;

    // Find the items that will be in the knapsack.
    for (i = TASK_COUNT; i > 0; i--)
    {
        if (aux_matrix[i - 1][k] != best)
        {
            items |= (1 << (i - 1));
            k = (k - weights[i - 1]);
            best = aux_matrix[i][k];
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
        if (tick >= task_deadline[i])
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
            values[i] = UINT32_MAX;
        }
        else
        {
            // Priority calculation.
            values[i] = max(1, (100UL - ((100UL * (task_deadline[i] - tick)) / task_period[i])));
        }
        // If the task has already been executed.
        if (task_already_executed[i] == true)
        {
            values[i] = 1;
        }
    }
    if (TASK_COUNT >= 4)
    {
        if (task_already_executed[4] == true)
        {
            values[4] = 0;
        }
    }

    tick += 1;

    return deadline_loss_counter;
}


uint32_t perturb_observe(uint32_t PspTotalNew, uint32_t Pload)
{
    static uint32_t PspTotal = 0UL, PspTotalOld = 0UL;
    static uint32_t perturb = 0, perturbOld = 0;
    static const uint32_t step = 1;
    uint32_t perturbAux;
    uint32_t W;

    PspTotalOld = PspTotal;
    PspTotal = PspTotalNew;
    perturbAux = perturbOld;
    perturbOld = perturb;

    if ((PspTotalOld > PspTotal) && (perturbOld == 0))
    {
        perturb = 1;
    }
    else if ((PspTotalOld > PspTotal) && (perturbOld == 1))
    {
        perturb = 0;
    }

    if ((PspTotalOld < PspTotal) && (perturbOld == 0))
    {
        perturb = 0;
    }
    else if ((PspTotalOld < PspTotal) && (perturbOld == 1))
    {
        perturb = 1;
    }

    if (PspTotalOld == PspTotal)
    {
        perturb = perturbAux;
    }

    (perturb) ? (W = Pload - step) : (W = Pload + step);
    if (W <= 1) { W = 1; }

    return W;
}

