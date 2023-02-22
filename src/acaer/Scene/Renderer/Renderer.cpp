/**
 * @file Renderer.cpp
 * @author flaxhaufen
 * @brief 
 * @version 0.1
 * @date 2023-02-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Scene/Renderer/Renderer.h"
#include "acaer/Scene/Entity.h"
#include "acaer/Scene/ScriptableEntity.h"


// *** DEFINE ***
#define AC_OUTLINE_THICKNESS        1
#define AC_ORIGIN_POINT_RADIUS      2.f

// *** NAMESPACE ***
namespace Acaer {


    void Renderer::RenderSprite(sf::RenderWindow &window, Transform_C &transform_c, Sprite_C &sprite_c, b8 renderOutline, b8 renderOrigin) {

        sf::Sprite sprite;
        sprite.setTexture(sprite_c.texture);

        // Scale texture to size
        sprite.setScale({AC_GLOBAL_SCALE, AC_GLOBAL_SCALE});
        sprite.setPosition(sf::Vector2f(transform_c.pos.x, transform_c.pos.y));
        sprite.setRotation(transform_c.rotation);
        window.draw(sprite);

        if (renderOutline) {
            sf::RectangleShape rec;
            rec.setPosition(sf::Vector2f(transform_c.pos.x, transform_c.pos.y));
            rec.setRotation(transform_c.rotation);
            // rec.setOrigin(sf::Vector2f(t.size.x / 2.f, t.size.y / 2.f));
            rec.setSize(sf::Vector2f(transform_c.size.x, transform_c.size.y));

            rec.setFillColor(sf::Color(0, 0, 0 , 0));       // Setting the fillcolor to nothing
            rec.setOutlineThickness(AC_OUTLINE_THICKNESS);
            rec.setOutlineColor(sf::Color::White);
            window.draw(rec);
        }

        if (renderOrigin) {
            // Render origin
            sf::CircleShape c;
            c.setRadius(AC_ORIGIN_POINT_RADIUS);
            c.setFillColor(sf::Color::Red);
            c.setPosition(sf::Vector2f(transform_c.pos.x - AC_ORIGIN_POINT_RADIUS, transform_c.pos.y - AC_ORIGIN_POINT_RADIUS));
            window.draw(c);
        }
    }
}