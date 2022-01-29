/**
 * @file rand_generator.c
 * @author Vinicius Pimenta Bernardo (viniciuspibi@gmail.com)
 * @brief Random number generator.
 * @version 0.1
 * @date 2022-01-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>

#include "rand_generator.h"

uint32_t rand_generator()
{
    uint32_t numero = rand()%100;
    return numero;
}
