/**
 * @file CharacterController.cpp
 * @author flexhaufen
 * @brief Character controller for player
 * @version 0.1
 * @date 2023-02-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/ScriptableEntity.h"

// *** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Character Controller script
     * 
     */
    class CharacterController: public ScriptableEntity {
    public:
        void OnUpdate(f32 dt) {
            auto& t = GetComponent<Transform_C>();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {t.pos.y -= 200.f * dt;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {t.pos.y += 200.f * dt;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {t.pos.x -= 200.f * dt;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {t.pos.x += 200.f * dt;}
        }
    };

}