/**
 * @file Renderer.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-02-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
# pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Components.h"

//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    namespace Renderer {


        /**
         * @brief Renders given sprite
         * 
         * @param window        sf::RenderWindow reference
         * @param transform_c   transform
         * @param sprite_c      sprite to render
         * @param renderOutline true: Render outline of sprite
         * @param renderOrigin  true: Render origin of transform
         */
        void RenderSprite(sf::RenderWindow &window, Transform_C &transform_c, const Sprite_C &sprite_c, b8 renderOutline = false, b8 renderOrigin = false);
   
        /**
         * @brief Render hitbox of given collider
         * 
         * @param window        sf::RenderWindow reference
         * @param transform_c   transform
         * @param collider_c    collider
         */
        void RenderHitbox(sf::RenderWindow &window, Transform_C &transform_c, Collider_C &collider_c);
    }
}