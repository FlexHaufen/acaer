/**
 * @file Types.h
 * @author flexhaufen
 * @brief Custom Types
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include <cstdint>


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

// bool
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

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}