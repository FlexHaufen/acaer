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
#include "acaer/Scene/Entity/Entity.h"
#include "acaer/Scene/Entity/ScriptableEntity.h"

#include "acaer/Helper/Convert/Convert.h"


// *** NAMESPACE ***
namespace Acaer {

    void Renderer::RenderSprite(sf::RenderWindow &window, Component::Sprite &sprite) {
        window.draw(sprite.spriteTexture);
    }

    void Renderer::RenderCell(sf::RenderWindow &window, size_t x, size_t y, vColor c) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f((f32)x * AC_GLOBAL_SCALE, (f32)y  * AC_GLOBAL_SCALE));
        rec.setSize(sf::Vector2f(1 * AC_GLOBAL_SCALE, 1 * AC_GLOBAL_SCALE));

        rec.setFillColor(Convert::vColor_to_sfColor(c));       // Setting the fillcolor to nothing
        window.draw(rec);
    }
}