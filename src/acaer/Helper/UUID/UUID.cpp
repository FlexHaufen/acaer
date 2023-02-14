/**
 * @file UUID.cpp
 * @author flexhaufen
 * @brief UUID
 * @version 0.1
 * @date 2023-02-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Helper/UUID/UUID.h"

#include <random>

// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<u64> s_UniformDistribution;


    UUID::UUID() {
        m_UUID = s_UniformDistribution(s_Engine);
    }


    UUID::UUID(u64 uuid) {
        m_UUID = uuid;
    }
}