/**
 * @file DebugRenderer.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"

#include "acaer/Helper/Convert/Convert.h"



//*** NAMESPACE ***
namespace Acaer {

    /**
     * @brief DebugRenderer
     * 
     */
    class DebugRenderer : public b2Draw {

    public:

        /**
         * @brief Constructor Duh!
         * 
         * @param window        sf::RenderWindow reference

         */
        DebugRenderer(sf::RenderWindow& window);

        // *** OVERRIDE FUNCTIONS FOR BOX2D ***
        void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

        void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

        void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);

        // TODO (flex): Implement
        void DrawPoint(const b2Vec2& p, float size, const b2Color& color) { return; }

        void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);

        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

        void DrawTransform(const b2Transform& xf);


        // *** CUSTOM ***
        void RenderMouseCoords(const sf::Vector2f &pos);

        /**
         * @brief Render the origin point of given transform
         *
         * @param transform_c   transform
         */
        void RenderTransformOrigin(const Component::Transform &transform_c);

        /**
         * @brief Render the outline of given sprite
         *
         * @param transform_c   transform
         * @param sprite_c      sprite
         */
        void RenderSpriteOutline(const Component::Transform &transform_c, const Component::Sprite &sprite_c);

        /**
         * @brief Render the cunck outline border
         * 
         * @param sizeX         Chunksize x
         * @param sizeY         Chunksize y
         * @param posX          Chunkpos x
         * @param posY          Chunkpos y
         */
        void RenderChunkBorder(size_t sizeX, size_t sizeY, int posX, int posY);

        /**
         * @brief Render Dirty Rectangle of simulated Chunk
         * 
         * @param minPos        maximal pos of rect
         * @param maxPos        minimal pos of rect
         */
        void RenderChunkDirtyRect(v2<s32> minPos, v2<s32> maxPos);

    private:
    
        /**
         * @brief Renders a Rect
         *        The fill color will apear slightly transparent (alpha = 100)
         *
         * @param rec           sf::RectangleShape reference
         * @param color         color of rectangle
         * @param isFilled      render filled rectangle
         */
        void RenderRectWithOutline(sf::RectangleShape &rec, sf::Color color, b8 isFilled);

    private:

        // ** Members **
        sf::RenderWindow& m_Window;     // Reference to your SFML window
        sf::Font m_Font;                // Font for debug renderer
    };
}