/**
 * @file DebugRenderer.cpp
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Scene/Renderer/DebugRenderer.h"


// *** DEFINE ***
#define AC_OUTLINE_THICKNESS        1

#define AC_SPRITE_OUTLINE_COLOR     sf::Color(sf::Color::White)

#define AC_CHUNK_THICKNESS          1
#define AC_CHUNK_COLOR_OUTLINE      sf::Color(255, 100, 0, 255) // Orange
#define AC_CHUNK_RECT_COLOR_OUTLINE sf::Color(  0, 128, 0, 255) // Green

#define AC_ORIGIN_POINT_RADIUS      2.f


// *** NAMESPACE ***
namespace Acaer {


    DebugRenderer::DebugRenderer(sf::RenderWindow& window) : m_Window(window) {
        // Set the drawing flags
        SetFlags(e_shapeBit | e_jointBit);
    }

    void DebugRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
        sf::ConvexShape shape;
        shape.setPointCount(vertexCount);
        for (s32 i = 0; i < vertexCount; ++i) {
            const b2Vec2& vertex = vertices[i];
            shape.setPoint(i, sf::Vector2f(vertex.x, vertex.y));
        }

        sf::Color sfColor = Convert::b2Color_to_sfColor(color);
        sfColor.a = 255;
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sfColor);
        shape.setOutlineThickness(1.0f);
        shape.setScale(AC_PPM, AC_PPM);

        m_Window.draw(shape);
    }

    void DebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
        sf::ConvexShape shape;
        shape.setPointCount(vertexCount);
        for (s32 i = 0; i < vertexCount; ++i) {
            const b2Vec2& vertex = vertices[i];
            shape.setPoint(i, sf::Vector2f(vertex.x, vertex.y));
        }

        sf::Color sfColor = Convert::b2Color_to_sfColor(color);
        sfColor.a = 128;
        shape.setFillColor(sfColor);
        shape.setScale(AC_PPM, AC_PPM);


        m_Window.draw(shape);
    }

    void DebugRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {
        sf::CircleShape shape(radius);
        shape.setFillColor(sf::Color::Transparent);

        sf::Color sfColor = Convert::b2Color_to_sfColor(color);
        sfColor.a = 255;
        shape.setOutlineColor(sfColor);
        shape.setOutlineThickness(1.0f);

        shape.setPosition(center.x - radius, center.y - radius);
        shape.setScale(AC_PPM, AC_PPM);

        m_Window.draw(shape);
    }

    void DebugRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {
        sf::CircleShape shape(radius);
        shape.setFillColor(sf::Color::Transparent);

        sf::Color sfColor = Convert::b2Color_to_sfColor(color);
        sfColor.a = 128;
        shape.setFillColor(sfColor);

        shape.setPosition(center.x - radius, center.y - radius);
        shape.setScale(AC_PPM, AC_PPM);

        m_Window.draw(shape);
    }

    void DebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(p1.x, p1.y)),
            sf::Vertex(sf::Vector2f(p2.x, p2.y))
        };

        sf::Color sfColor = Convert::b2Color_to_sfColor(color);
        sfColor.a = 255;
        line[0].color = sfColor;
        line[1].color = sfColor;

        m_Window.draw(line, 2, sf::Lines);
    }

    void DebugRenderer::DrawTransform(const b2Transform& xf) {
        const f32 k_axisScale = 0.4f;

        b2Vec2 p1 = xf.p, p2;

        p2 = p1 + k_axisScale * xf.q.GetXAxis();
        DrawSegment(p1, p2, b2Color(1, 0, 0));

        p2 = p1 + k_axisScale * xf.q.GetYAxis();
        DrawSegment(p1, p2, b2Color(0, 1, 0));
    }

    void DebugRenderer::RenderTransformOrigin(sf::RenderWindow &window, const Component::Transform &transform_c){
        sf::CircleShape c;
        c.setRadius(AC_ORIGIN_POINT_RADIUS);
        c.setFillColor(sf::Color::Red);
        c.setPosition(sf::Vector2f(transform_c.pos.x - AC_ORIGIN_POINT_RADIUS, transform_c.pos.y - AC_ORIGIN_POINT_RADIUS));
        window.draw(c);
    }

    void DebugRenderer::RenderSpriteOutline(sf::RenderWindow &window, const Component::Transform &transform_c, const Component::Sprite &sprite_c) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f(transform_c.pos.x, transform_c.pos.y));
        rec.setRotation(transform_c.rotation);

        // NOTE (flex): d_with & d_heigt need to be absolute
        rec.setSize(sf::Vector2f(abs((f32)sprite_c.spriteTexture.getTextureRect().width)  * AC_GLOBAL_SCALE,
                                abs((f32)sprite_c.spriteTexture.getTextureRect().height) * AC_GLOBAL_SCALE));
        RenderRectWithOutline(window, rec, AC_SPRITE_OUTLINE_COLOR, false); 
    }

    void DebugRenderer::RenderChunkBorder(sf::RenderWindow &window, size_t sizeX, size_t sizeY, int posX, int posY) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f((f32)posX * AC_GLOBAL_SCALE, (f32)posY * AC_GLOBAL_SCALE));
        rec.setSize(sf::Vector2f(sizeX * AC_GLOBAL_SCALE, sizeY * AC_GLOBAL_SCALE));

        rec.setFillColor({0,0,0,0});       // Setting the fillcolor to nothing
        rec.setOutlineThickness(AC_CHUNK_THICKNESS);
        rec.setOutlineColor(AC_CHUNK_COLOR_OUTLINE);
        window.draw(rec);
    }

    void DebugRenderer::RenderChunkDirtyRect(sf::RenderWindow &window, v2<s32> minPos, v2<s32> maxPos) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f((f32)minPos.x * AC_GLOBAL_SCALE, (f32)minPos.y * AC_GLOBAL_SCALE));
        rec.setSize(sf::Vector2f((maxPos.x - minPos.x) * AC_GLOBAL_SCALE, (maxPos.y - minPos.y) * AC_GLOBAL_SCALE));

        rec.setFillColor({0,0,0,0});       // Setting the fillcolor to nothing
        rec.setOutlineThickness(AC_CHUNK_THICKNESS);
        rec.setOutlineColor(AC_CHUNK_RECT_COLOR_OUTLINE);
        window.draw(rec);
    }

    void DebugRenderer::RenderRectWithOutline(sf::RenderWindow &window, sf::RectangleShape &rec, sf::Color color, b8 isFilled) {
        if (isFilled) {
            rec.setFillColor(sf::Color(color.a, color.g, color.b, 100));       // Setting the fillcolor to nothing
        }
        else {
            rec.setFillColor(sf::Color(0, 0, 0, 0));    // transparent
        }
        rec.setOutlineColor(color);
        rec.setOutlineThickness(AC_OUTLINE_THICKNESS);
        window.draw(rec);
    }

}