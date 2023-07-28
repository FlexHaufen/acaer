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

//#include "acaer/Scene/World/WorldChunk.h"

//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Renderer Class
     * 
     */
    class Renderer {
    public:

        Renderer(sf::RenderWindow &window) : m_Window(window) {}

        void RenderSprite(const Component::Sprite &sprite_c);

        /**
         * @brief Renders a single Cell
         *
         * @param x             pos x
         * @param y             pos y
         * @param c             color of cell (rgba)
         */
        void RenderCell(s32 x, s32 y, vColor c);

    private:
        // ** Memberse **
        sf::RenderWindow   &m_Window;   // Ref to sf::RenderWindow

    };
}