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

        DebugRenderer(sf::RenderWindow& window) : m_Window(window) {
            // Set the drawing flags
            SetFlags(e_shapeBit | e_jointBit);
        }

        void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
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

        void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
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

        void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {
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

        void DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
            return;
        }

        void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {
            sf::CircleShape shape(radius);
            shape.setFillColor(sf::Color::Transparent);

            sf::Color sfColor = Convert::b2Color_to_sfColor(color);
            sfColor.a = 128;
            shape.setFillColor(sfColor);

            shape.setPosition(center.x - radius, center.y - radius);
            shape.setScale(AC_PPM, AC_PPM);

            m_Window.draw(shape);
        }

        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
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

        void DrawTransform(const b2Transform& xf) {
            const f32 k_axisScale = 0.4f;

            b2Vec2 p1 = xf.p, p2;

            p2 = p1 + k_axisScale * xf.q.GetXAxis();
            DrawSegment(p1, p2, b2Color(1, 0, 0));

            p2 = p1 + k_axisScale * xf.q.GetYAxis();
            DrawSegment(p1, p2, b2Color(0, 1, 0));
        }

        /*
        void DrawShapeAsSensor(b2Fixture* fixture) {
            b2Shape* shape = fixture->GetShape();

            switch (shape->GetType()) {
                case b2Shape::e_circle: {
                    b2CircleShape* circle = dynamic_cast<b2CircleShape*>(shape);
                    b2Transform transform = fixture->GetBody()->GetTransform();
                    b2Vec2 center = b2Mul(transform, circle->m_p);
                    float radius = circle->m_radius;
                    DrawCircle(center, radius, b2Color(0, 0, 1));
                    break;
                }
                case b2Shape::e_polygon: {
                    b2PolygonShape* polygon = dynamic_cast<b2PolygonShape*>(shape);
                    b2Transform transform = fixture->GetBody()->GetTransform();
                    int32 vertexCount = polygon->m_count;
                    b2Vec2 vertices[b2_maxPolygonVertices];
                    for (int32 i = 0; i < vertexCount; ++i) {
                        vertices[i] = b2Mul(transform, polygon->m_vertices[i]);
                    }
                    DrawPolygon(vertices, vertexCount, b2Color(0, 0, 1));
                    break;
                }
                case b2Shape::e_chain: {
                    b2ChainShape* chain = dynamic_cast<b2ChainShape*>(shape);
                    b2Transform transform = fixture->GetBody()->GetTransform();
                    int32 vertexCount = chain->m_count;
                    b2Vec2 vertices[100];   // TODO (flex) make define
                    for (int32 i = 0; i < vertexCount; ++i) {
                        vertices[i] = b2Mul(transform, chain->m_vertices[i]);
                    }
                    DrawPolygon(vertices, vertexCount, b2Color(0, 0, 1));
                    break;
                }
                default:
                    break;
            }
        }
        */

    private:
        sf::RenderWindow& m_Window; // Reference to your SFML window
    };
}