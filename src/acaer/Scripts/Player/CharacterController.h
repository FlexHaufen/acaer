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
#include "acaer/Scene/Entity/ScriptableEntity.h"

// *** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Character Controller script
     * 
     */
    class CharacterController: public ScriptableEntity {
    public:
        void OnUpdate(f32 dt) {
            auto& rb = GetComponent<RigidBody_C>();


            // TODO: use force instead of lin. vel.
            b2Vec2 vel = rb.RuntimeBody->GetLinearVelocity();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {vel.y = -10.f;}
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {rb.RuntimeBody->SetLinearVelocity({    0,  10.f});}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))    {vel.x = -5.f;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))    {vel.x = 5.f;}
            
            rb.RuntimeBody->SetLinearVelocity({vel.x, vel.y});

            //auto& t = GetComponent<Transform_C>();
            //AC_CORE_TRACE("player pos x: {0} | pos y: {1}", t.pos.x, t.pos.y);
        }
    };

}