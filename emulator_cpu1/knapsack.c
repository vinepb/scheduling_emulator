/**
 * @file knapsack.c
 * @brief Dynamic knapsack algorithm implementation.
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdint.h>
#include "knapsack.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MAX_PRIORITY UINT16_MAX

const uint16_t          task_power_consumption[TASK_COUNT];
const uint16_t          task_computing_time[TASK_COUNT];
const uint16_t          task_period[TASK_COUNT];
volatile uint8_t        task_dynamic_priority[TASK_COUNT];
volatile uint8_t        task_on_off_status[TASK_COUNT];
volatile uint16_t       task_deadline[TASK_COUNT];
volatile uint16_t       task_executed_time[TASK_COUNT];
volatile uint8_t        task_already_executed[TASK_COUNT];

#define r task_power_consumption
#define c task_computing_time
#define dl task_period
#define u task_dynamic_priority
#define x task_on_off_status
#define d task_deadline
#define a task_executed_time
#define ex task_already_executed

void dynamic_priority(void)
{
	for (int i = 0; i < TASK_COUNT; i++)
	{
		/* Did the task fineshed */
		if (c[i] - a[i] <= 0)
		{
			/* Variablie initialization */
			x[i] = 0;
			a[i] = 0;
			u[i] = 0;
			ex[i] = 0;
		}
		/* New deadline calculation */
		if (/* TODO: epoch */ >= d[i])
		{
			d[i] += dl[i];
			ex[i] = 0;
		}
		/* Is task running */
		if (x[i] == 1)
		{
			a[i] += /* TODO: elapsed time */;
		}
		/* Next deadline loss */
		if (d[i] - c[i] - /* TODO: epoch */ <= 0)
		{
			u[i] = MAX_PRIORITY;
		}
		else
		{
			/* Priority calculation */
			u[i] = 100 - ((100 * (d[i] - /* TODO: epoch */)) / dl[i]);
		}
		/* If the task has already finished */
		if (ex[i] == 1)
		{
			u[i] = 1;
		}
	}
}

uint16_t dynamic_knapsack(uint16_t max_power_consumption)
{
	#define w max_power_consumption
	uint16_t (*m)[TASK_COUNT] = malloc(sizeof(uint16_t) * (w + 1));
	/* Intermediate results matrix initialization */
	for (int i = 0; i < TASK_COUNT; i++)
	{
		for (int j = 0; j <= w; j++)
		{
			m[i][j] = 0;
		}
	}
	for (int i = 1; i < TASK_COUNT; i++)
	{
		for (int j = 0; j <= w; j++)
		{
			if (r[i] > j)
			{
				m[i][j] = m[i - 1][j];
			}
			else
			{
				m[i][j] = MAX(m[i - 1][j], m[i - 1][j - r[i]] + u[i]);
			}
		}
	}
	/* Optimal solution */
	uint16_t best = m[TASK_COUNT][w];
	uint16_t k = w;
	for (int i = TASK_COUNT; i > 0; i--)
	{
		if (m[i - 1][k] == best)
		{
			x[i-1] = 0;
		}
		else
		{
			x[i-1] = 1;
			k -= r[i-1];
			best = m[i-1][w]; // ( = m[i-1][k] ?)
		}
	}
	for (int i = 0; i < TASK_COUNT; i++)
	{
		free(m[i]);
	}
	free(m);
	return best;
}
