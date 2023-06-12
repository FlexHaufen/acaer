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

        //! ---------------------------------- DEBUG ----------------------------------
        /**
         * @brief Render collider component
         *
         * @param window        sf::RenderWindow reference
         * @param transform_c   transform
         * @param collider_c    collider
         */
        void RenderCollider(sf::RenderWindow &window, Component::Transform &transform_c, Component::Collider &collider_c);

        /**
         * @brief Renders all sensors components of given collider
         *
         * @param window        sf::RenderWindow reference
         * @param transform_c   transform
         * @param collider_c    collider
         */
        void RenderSensors(sf::RenderWindow &window, Component::Transform &transform_c, Component::Collider &collider_c);

        /**
         * @brief Render the origin point of given transform
         *
         * @param window        sf::RenderWindow reference
         * @param transform_c   transform
         */
        void RenderTransformOrigin(sf::RenderWindow &window, Component::Transform &transform_c);

        /**
         * @brief Render the outline of given sprite
         *
         * @param window        sf::RenderWindow reference
         * @param transform_c   transform
         * @param sprite_c      sprite
         */
        void RenderSpriteOutline(sf::RenderWindow &window, Component::Transform &transform_c, const Component::Sprite &sprite_c);

        // TODO: Add description
        void RenderChunkBorder(sf::RenderWindow &window, size_t sizeX, size_t sizeY, int posX, int posY);
        void RenderChunkDirtyRect(sf::RenderWindow &window, v2<s32> minPos, v2<s32> maxPos);
        //! ---------------------------------------------------------------------------

    private:

        /**
         * @brief Renders a Rect
         *        The fill color will apear slightly transparent (alpha = 100)
         *
         * @param window        sf::RenderWindow reference
         * @param rec           sf::RectangleShape reference
         * @param color         color of rectangle
         * @param isFilled      render filled rectangle
         */
        void RenderRectWithOutline(sf::RenderWindow &window, sf::RectangleShape &rec, sf::Color color, b8 isFilled);
    };
}