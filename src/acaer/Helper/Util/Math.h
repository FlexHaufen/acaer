/**
 * @file Math.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include <math.h>
#include <random>


/**
 * @brief generate random int between 0..max
 * @warning This function requires a seed for rand() to be set
 *          e.x. srand(time(NULL))
 * 
 * @param max   Maximum random number
 * @return int  Random number
 */
int rand_i(int max) {
    if (max < 0) {
        return -(rand() % (abs(max) + 1));
    }
    return rand() % (max + 1);
}
