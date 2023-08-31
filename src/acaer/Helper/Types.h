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
struct v2{
    T x;
    T y;

    // ** Operator Overloading **
    inline v2<T> operator+ (const v2<T>& a) const { return {x + a.x, y + a.y}; }
    inline v2<T> operator- (const v2<T>& a) const { return {x - a.x, y - a.y}; }
    inline v2<T> operator* (const v2<T>& a) const { return {x * a.x, y * a.y}; }
    inline v2<T> operator/ (const v2<T>& a) const { return {x / a.x, y / a.y}; }
    inline v2<T> operator= (const v2<T>& a)       { x = a.x; y = a.y; return a; }
    inline bool operator== (const v2<T>& a) const { return (x == a.x && y == a.y); }
};

/**
 * @brief 2D vector
 * 
 */
struct v2f {
    f32 x;
    f32 y;

    // ** Operator Overloading **
    inline v2f operator+ (const v2f& a) const { return {x + a.x, y + a.y}; }
    inline v2f operator- (const v2f& a) const { return {x - a.x, y - a.y}; }
    inline v2f operator* (const v2f& a) const { return {x * a.x, y * a.y}; }
    inline v2f operator/ (const v2f& a) const { return {x / a.x, y / a.y}; }
    inline v2f operator= (const v2f& a)       { x = a.x; y = a.y; return a; }
    inline bool operator== (const v2f& a) const { return (x == a.x && y == a.y); }
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

    // ** Operator Overloading **
    inline v3<T> operator+ (const v3<T>& a) const { return {x + a.x, y + a.y, z + a.z}; }
    inline v3<T> operator- (const v3<T>& a) const { return {x - a.x, y - a.y, z - a.z}; }
    inline v3<T> operator* (const v3<T>& a) const { return {x * a.x, y * a.y, z * a.z}; }
    inline v3<T> operator/ (const v3<T>& a) const { return {x / a.x, y / a.y, z / a.z}; }
    inline v3<T> operator= (const v3<T>& a)       { x = a.x; y = a.y; z = a.z; return a; }
    inline bool operator== (const v3<T>& a) const { return (x == a.x && y == a.y && z == a.z); }
};

/**
 * @brief 4D Vector
 * 
 * @tparam T type of vector
 */
template <typename T>
struct v4 {
    T w;
    T x;
    T y;
    T z;
    
    // ** Operator Overloading **
    inline v4<T> operator+ (const v4<T>& a) const { return {w + a.w, x + a.x, y + a.y, z + a.z}; }
    inline v4<T> operator- (const v4<T>& a) const { return {w - a.w, x - a.x, y - a.y, z - a.z}; }
    inline v4<T> operator* (const v4<T>& a) const { return {w * a.w, x * a.x, y * a.y, z * a.z}; }
    inline v4<T> operator/ (const v4<T>& a) const { return {w / a.w, x / a.x, y / a.y, z / a.z}; }
    inline v4<T> operator= (const v4<T>& a)       { w = a.w; x = a.x; y = a.y; z = a.z; return a; }
    inline bool operator== (const v4<T>& a) const { return (w == a.w && x == a.x && y == a.y && z == a.z); }
};

/**
 * @brief Color Vector
 * 
 */
struct vColor {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    
    // FIXME (inj)
    /*
    // ** Operator Overloading **
    inline vColor operator+ (const vColor& a) const { return {r + a.r, g + a.g, b + a.b, a + a.a}; }
    inline vColor operator- (const vColor& a) const { return {r - a.r, g - a.g, b - a.b, a - a.a}; }
    inline vColor operator* (const vColor& a) const { return {r * a.r, g * a.g, b * a.b, a * a.a}; }
    inline vColor operator/ (const vColor& a) const { return {r / a.r, g / a.g, b / a.b, a / a.a}; }
    inline vColor operator= (const vColor& a)       { r = a.r; g = a.g; b = a.b; a = a.a; return a; }
    inline bool  operator== (const vColor& a) const { return (r == a.r && g == a.g && b == a.b && a == a.a); }
    */
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