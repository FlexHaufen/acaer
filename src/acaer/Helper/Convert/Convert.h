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
         * @param rb        RigidBody_C
         * @param t         Transform_C
         * @param c         Collider_C
         * @param world     Physics World
         */
        void create_b2Body(RigidBody_C &rb,Transform_C &t, Collider_C &c, b2World *world) ;

        /**
         * @brief Get the Position from b2Body
         * 
         * @param body  b2Body
         * @param c     Collider_C
         * @return v2f  {pos.x, pos.y}
         */
        v2f getPositionFrom_b2Body(b2Body* body, Collider_C c);

        /**
         * @brief Get the Rotation from b2Body
         * 
         * @param body  b2Body
         * @return f32  rotation
         */
        f32 getRotationFrom_b2Body(b2Body* body);

        /**
         * @brief Converts sf::Color to vColor
         * 
         * @param c         sf::Color
         * @return vColor   vColor
         */
        vColor sfColor_to_vColor(sf::Color c);

        /**
         * @brief Converts vColor to sf::Color
         * 
         * @param c         vColor
         * @return vColor   sf::Color
         */
        sf::Color vColor_to_sfColor(vColor c);
    }
}