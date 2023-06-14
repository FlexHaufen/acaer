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
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Entity/Components.h"

//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Renderer Class
     * 
     */
    class Renderer {

    public:

        void RenderSprite(sf::RenderWindow &window, Component::Sprite &sprite_c);

        /**
         * @brief Renders a single Cell
         *
         * @param window        sf::RenderWindow reference
         * @param x             pos x
         * @param y             pos y
         * @param c             color of cell (rgba)
         */
        void RenderCell(sf::RenderWindow &window, size_t x, size_t y, vColor c);
    };
}