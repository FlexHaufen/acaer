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

        /**
         * @brief Sets up given sprite component
         *        for game
         * @param sprite        sprite
         * @param tag           tag
         */
        void OnStart(Component::Sprite &sprite, const Component::Tag &tag);

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
        AnimationHandler m_AnimationHandler;
    };
}