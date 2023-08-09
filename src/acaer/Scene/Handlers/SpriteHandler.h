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
#include "acaer/Scene/Handlers/TextureHandler.h"

//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {

    class SpriteHandler {
    public:

        SpriteHandler();

        /**
         * @brief Create new Sprite object
         * 
         * @param sprite    Sprite Component
         * @param tag       Tag Component
         */
        void CreateSprite(Component::Sprite &sprite, const Component::Tag &tag);

        /**
         * @brief Delete Given Sprite
         * 
         * @param sprite    Sprite Component
         */
        void DeleteSprite(Component::Sprite &sprite);

        /**
         * @brief Update Static Sprites (Non Animated)
         * 
         * @param sprite        sprite
         * @param transform     transform
         */
        void HandleStaticSprite(Component::Sprite &sprite, const Component::Transform &transform);

        /**
         * @brief Update Dynamic Sprites (Animated)
         * 
         * @param dt                delta time
         * @param sprite            sprite
         * @param spriteAnimator    spriteAnimator
         * @param transform         transform
         */
        void HandleDynamicSprite(f32 dt, Component::Sprite &sprite, Component::SpriteAnimatior &spriteAnimator, const Component::Transform &transform);

    private:
        // ** Memebers **
        Ref<AnimationHandler>   m_AnimationHandler;
        Ref<TextureHandler>     m_TextureHandler; 
    };
}