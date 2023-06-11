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


// *** DEFINE ***
#define AC_OUTLINE_THICKNESS        1

#define AC_SPRITE_OUTLINE_COLOR     sf::Color(sf::Color::White)
#define AC_COLLIDER_COLOR           sf::Color(235,  64,  52, 255) // Red
#define AC_SENSOR_COLOR             sf::Color(  5, 252,  46, 255) // Green

#define AC_CHUNK_THICKNESS          1
#define AC_CHUNK_COLOR_OUTLINE      sf::Color(255, 100, 0, 255) // Orange
#define AC_CHUNK_RECT_COLOR_OUTLINE sf::Color(  0, 128, 0, 255) // Green



#define AC_ORIGIN_POINT_RADIUS      2.f


// *** NAMESPACE ***
namespace Acaer {

    void Renderer::InitialzeSprites(Component::SpriteAnimaton &sprite) {
        if (sprite.texture.getSize() == sf::Vector2u(0, 0)) {
            // TODO (flex): Add Default texture
            // TODO (flex): Use tag component to identify sprite
            AC_CORE_WARN("Renderer could not find valid texture");
            return;
        }
        sprite.spriteTexture.setTexture(sprite.texture);

        if (sprite.isAnimated) {
            sprite.totalFrames = (sprite.gridSize.x * sprite.gridSize.y);
        }
    }

    void Renderer::RenderSpriteAnimaton(f32 dt, sf::RenderWindow &window, Component::Transform &transform_c, Component::SpriteAnimaton &sprite) {
        if (sprite.isAnimated) {
            
            sprite.elapsedTime += dt;

            if (sprite.elapsedTime >= 1.0f / sprite.animationSpeed) {
                sprite.currentFrame = (sprite.currentFrame + 1) % sprite.totalFrames;    
                // Calculate coords
                v2<s16> coords = {
                    .x = (sprite.currentFrame % sprite.gridSize.y) * sprite.frameSize.x,
                    .y = (sprite.currentFrame / sprite.gridSize.y) * sprite.frameSize.y
                };
                sprite.spriteTexture.setTextureRect(sf::IntRect(coords.x, coords.y, sprite.frameSize.x, sprite.frameSize.y));
                sprite.elapsedTime = 0.f;
            }
        }
        sprite.spriteTexture.setScale({AC_GLOBAL_SCALE, AC_GLOBAL_SCALE});
        sprite.spriteTexture.setPosition(sf::Vector2f(transform_c.pos.x, transform_c.pos.y));
        sprite.spriteTexture.setRotation(transform_c.rotation);
        window.draw(sprite.spriteTexture);
    }

    void Renderer::RenderCell(sf::RenderWindow &window, size_t x, size_t y, vColor c) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f((f32)x * AC_GLOBAL_SCALE, (f32)y  * AC_GLOBAL_SCALE));
        rec.setSize(sf::Vector2f(1 * AC_GLOBAL_SCALE, 1 * AC_GLOBAL_SCALE));

        rec.setFillColor(Convert::vColor_to_sfColor(c));       // Setting the fillcolor to nothing
        window.draw(rec);
    }

    //! ---------------------------------- DEBUG ----------------------------------

    void Renderer::RenderCollider(sf::RenderWindow &window, Component::Transform &transform_c, Component::Collider &collider_c) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f(transform_c.pos.x + collider_c.offset.x, transform_c.pos.y + collider_c.offset.y));
        rec.setRotation(transform_c.rotation);
        rec.setSize(sf::Vector2f(collider_c.size.x, collider_c.size.y));
        RenderRectWithOutline(window, rec, AC_COLLIDER_COLOR, true);            
    }

    void Renderer::RenderSensors(sf::RenderWindow &window, Component::Transform &transform_c, Component::Collider &collider_c) {
        for (auto [key, val] : collider_c.sensors) {
            sf::RectangleShape rec;
            rec.setPosition(sf::Vector2f(transform_c.pos.x + val.offset.x, transform_c.pos.y + val.offset.y));
            rec.setRotation(transform_c.rotation);
            rec.setSize(sf::Vector2f(val.size.x, val.size.y));
            RenderRectWithOutline(window, rec, AC_SENSOR_COLOR, true);            
        }
    }

    void Renderer::RenderTransformOrigin(sf::RenderWindow &window, Component::Transform &transform_c){
        sf::CircleShape c;
        c.setRadius(AC_ORIGIN_POINT_RADIUS);
        c.setFillColor(sf::Color::Red);
        c.setPosition(sf::Vector2f(transform_c.pos.x - AC_ORIGIN_POINT_RADIUS, transform_c.pos.y - AC_ORIGIN_POINT_RADIUS));
        window.draw(c);
    }

    void Renderer::RenderSpriteOutline(sf::RenderWindow &window, Component::Transform &transform_c, const Component::SpriteAnimaton &sprite_c) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f(transform_c.pos.x, transform_c.pos.y));
        rec.setRotation(transform_c.rotation);

        if (sprite_c.isAnimated) {
            rec.setSize(sf::Vector2f((f32)sprite_c.frameSize.y * AC_GLOBAL_SCALE, 
                                     (f32)sprite_c.frameSize.y * AC_GLOBAL_SCALE));
        }
        else {
            rec.setSize(sf::Vector2f((f32)sprite_c.texture.getSize().x * AC_GLOBAL_SCALE, 
                                     (f32)sprite_c.texture.getSize().y * AC_GLOBAL_SCALE));
        }
        RenderRectWithOutline(window, rec, AC_SPRITE_OUTLINE_COLOR, false); 
    }

    void Renderer::RenderChunkBorder(sf::RenderWindow &window, size_t sizeX, size_t sizeY, int posX, int posY) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f((f32)posX * AC_GLOBAL_SCALE, (f32)posY * AC_GLOBAL_SCALE));
        rec.setSize(sf::Vector2f(sizeX * AC_GLOBAL_SCALE, sizeY * AC_GLOBAL_SCALE));

        rec.setFillColor({0,0,0,0});       // Setting the fillcolor to nothing
        rec.setOutlineThickness(AC_CHUNK_THICKNESS);
        rec.setOutlineColor(AC_CHUNK_COLOR_OUTLINE);
        window.draw(rec);
    }

    void Renderer::RenderChunkDirtyRect(sf::RenderWindow &window, v2<s32> minPos, v2<s32> maxPos) {
        sf::RectangleShape rec;
        rec.setPosition(sf::Vector2f((f32)minPos.x * AC_GLOBAL_SCALE, (f32)minPos.y * AC_GLOBAL_SCALE));
        rec.setSize(sf::Vector2f((maxPos.x - minPos.x) * AC_GLOBAL_SCALE, (maxPos.y - minPos.y) * AC_GLOBAL_SCALE));

        rec.setFillColor({0,0,0,0});       // Setting the fillcolor to nothing
        rec.setOutlineThickness(AC_CHUNK_THICKNESS);
        rec.setOutlineColor(AC_CHUNK_RECT_COLOR_OUTLINE);
        window.draw(rec);
    }
    
    //! ---------------------------------------------------------------------------

    void Renderer::RenderRectWithOutline(sf::RenderWindow &window, sf::RectangleShape &rec, sf::Color color, b8 isFilled) {
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