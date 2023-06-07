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
            auto& rb = GetComponent<Component::RigidBody>();
            auto& c  = GetComponent<Component::Collider>();

            // TODO: implement jump flag
            if (m_CanJump) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    f32 impulse = rb.RuntimeBody->GetMass() * m_jumpImpulse;
                    rb.RuntimeBody->ApplyLinearImpulse(b2Vec2(0, -impulse), rb.RuntimeBody->GetWorldCenter(), true);
                }
            }
            
            // TODO: use force instead of lin. vel.
            b2Vec2 vel = rb.RuntimeBody->GetLinearVelocity();
            m_velFactor = 5.f; // Set std vel factor

            // ** Sprint **
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {m_velFactor = 8.f;}
            
            // ** Left / Right **
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))    {vel.x = -m_velFactor;}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))    {vel.x =  m_velFactor;}

            rb.RuntimeBody->SetLinearVelocity(vel);
        }

    private:
        // ** Members **
        b8 m_CanJump = true;
        f32 m_velFactor;

        const f32 m_jumpImpulse = 2.f;
    };

}