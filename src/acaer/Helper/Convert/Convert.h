/**
 * @file Convert.h
 * @author flexhaufen
 * @brief Converts diffrent stuff
 * @version 0.1
 * @date 2023-03-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Entity/Components.h"

// *** NAMESPACE ***
namespace Acaer {
    namespace Convert {


        /**
         * @brief Create a b2Body object
         * 
         * @param rb        RigidBody
         * @param t         Transform
         * @param cc        ColliderContainer
         * @param world     Physics World
         */
        void create_b2Body(Component::RigidBody &rb, const Component::Transform &t, const Component::Collider &cc, b2World *world) ;

        /**
         * @brief Get the Position from b2Body
         * 
         * @param body  b2Body
         * @param c     Collider
         * @return v2f  {pos.x, pos.y}
         */
        v2f getPositionFrom_b2Body(const b2Body* body, const Component::Collider c);

        /**
         * @brief Get the Rotation from b2Body
         * 
         * @param body  b2Body
         * @return f32  rotation
         */
        f32 getRotationFrom_b2Body(const b2Body* body);

        /**
         * @brief Converts sf::Color to vColor
         * 
         * @param c         sf::Color
         * @return vColor   vColor
         */
        vColor sfColor_to_vColor(const sf::Color c);

        /**
         * @brief Converts vColor to sf::Color
         * 
         * @param c         vColor
         * @return vColor   sf::Color
         */
        sf::Color vColor_to_sfColor(const vColor c);


        /**
         * @brief Coverts v2Color to sf::Color
         * 
         * @param c             b2Color
         * @return sf::Color    sf::Color
         */
        sf::Color b2Color_to_sfColor(const b2Color c);
    }
}