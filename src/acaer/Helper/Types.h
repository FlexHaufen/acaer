#include <cstdint>

#pragma once

// *** CUSTOM TYPE DEFINTIONS ***

// unsigned
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

// signed
typedef char                s8;
typedef short               s16;
typedef int                 s32;
typedef long long           s64;


typedef bool                b8;

// floatingpoints
typedef float               f32;
typedef double              f64;
typedef long double         f128;

/**
 * @brief 2D vector
 * 
 * @tparam T type of vector
 */
template <typename T>
struct v2 {
    T x;
    T y;
};

/**
 * @brief 3D Vector
 * 
 * @tparam T type of vector
 */
template <typename T>
struct v3 {
    T x;
    T y;
    T z;
};