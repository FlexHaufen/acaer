/**
 * @file SpriteHandler.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Entity/Components.h"
#include "acaer/Scene/Handlers/AnimationHandler.h"

//*** DEFINES ***
#define AC_ERROR_TEXTURE_PATH   "assets/Textures/Debug/error_texture.png"

//*** NAMESPACE ***
namespace Acaer {

    class SpriteHandler {
    public:
        //SpriteHandler();
        //~SpriteHandler();


        void OnStart(Component::Sprite &sprite, Component::Tag &tag) {

            if (!sprite.texture.loadFromFile(sprite.texturepath)) {
                AC_CORE_WARN("Renderer could not find valid texture of Entity:");
                AC_CORE_WARN("    - {0}: [{1}]", tag.tag, tag.uuid);

                if (!sprite.texture.loadFromFile(AC_ERROR_TEXTURE_PATH)) {
                    AC_CORE_ERROR("Failed to load error_texture");
                    return;
                }

            }
            sprite.spriteTexture.setTexture(sprite.texture);
            sprite.spriteTexture.setScale({AC_GLOBAL_SCALE, AC_GLOBAL_SCALE});
        }

        /**
         * @brief Update Static Sprites (Non Animated)
         * 
         * @param sprite        sprite
         * @param transform     transform
         */
        void HandleStaticSprite(Component::Sprite &sprite, const Component::Transform &transform) {
            sprite.spriteTexture.setPosition(sf::Vector2f(transform.pos.x, transform.pos.y));
            sprite.spriteTexture.setRotation(transform.rotation);
        
        }

        /**
         * @brief Update Dynamic Sprites (Animated)
         * 
         * @param dt                delta time
         * @param sprite            sprite
         * @param spriteAnimator    spriteAnimator
         * @param transform         transform
         */
        void HandleDynamicSprite(f32 dt, Component::Sprite &sprite, Component::SpriteAnimatior &spriteAnimator, const Component::Transform &transform) {
            //HandleStaticSprite(sprite, transform);
            m_AnimationHandler.OnUpdate(dt, sprite, spriteAnimator);
            sprite.spriteTexture.setRotation(transform.rotation);
            sprite.spriteTexture.setPosition(sf::Vector2f(transform.pos.x, transform.pos.y));
        }


    private:
        // ** Memebers **
        AnimationHandler m_AnimationHandler;
    };
}