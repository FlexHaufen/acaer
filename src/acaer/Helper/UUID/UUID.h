/**
 * @file UUID.h
 * @author flexhaufen
 * @brief UUID
 * @version 0.1
 * @date 2023-02-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"

#include <xhash>

// *** DEFINES ***


// *** NAMESPACE ***
namespace Acaer {

    /**
     * @brief UUID class
     * 
     */
    class UUID {
    public:
        UUID();
        UUID(u64 uuid);


        operator u64() const { return m_UUID; }

    private:
        // ** Members **
        u64 m_UUID;         // UUID
    };
}


namespace std {
    template<>
    struct hash<Acaer::UUID> {
        std::size_t operator()(const Acaer::UUID& uuid) const {
            return hash<u64>((u64)uuid);
        }
    }
}
