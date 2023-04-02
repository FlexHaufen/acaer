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
 * @brief generate random size_t between 0..max
 * @warning This function requires a seed for rand() to be set
 *          e.x. srand(time(NULL))
 * 
 * @param max      Maximum random number
 * @return size_t  Random number
 */
inline size_t rand_u(size_t max) {
    if (max < 0) {
        return -(rand() % (abs((int)max) + 1));
    }
    return rand() % (max + 1);
}

// Pair hash is defined as the (first hash * #) ^ second hash
struct pair_hash {
    template<typename T1, typename T2>
    size_t operator() (const std::pair<T1, T2>& pair) const {
        return ( std::hash<T1>()(pair.first) * 0x1f1f1f1f)
            ^ std::hash<T2>()(pair.second);
    }
};