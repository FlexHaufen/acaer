/**
 * @file AnimationHandler.h
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

//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    class AnimationHandler {
    public:
    
        void OnStart() { return; }

        /**
         * @brief Updates Animation to next state
         * 
         * @param dt                deltaTime
         * @param sprite            sprite
         * @param spriteAnimator    spriteAnimator
         */
        void OnUpdate(f32 dt, Component::Sprite &sprite, Component::SpriteAnimatior &spriteAnimator);
    };
}