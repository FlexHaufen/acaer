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
    
        void OnStart() {
            return;
        }

        void OnUpdate(f32 dt, Component::Sprite &sprite, Component::SpriteAnimatior &spriteAnimator) {

            // Get currently active animation
            Component::SpriteAnimatior::Animation &currentAnimation = spriteAnimator.pool[spriteAnimator.currentAnimation];
            currentAnimation.elapsedTime += dt;
            if (currentAnimation.elapsedTime >= 1.0f / currentAnimation.animationSpeed) {

                currentAnimation.currentFrame = (currentAnimation.currentFrame + 1) % currentAnimation.frameLenght;    

                // Calculate pos of current frame
                v2<s32> frameCoord = {
                    .x = (currentAnimation.currentFrame % currentAnimation.frameLenght) * currentAnimation.frameSize.x,
                    .y = (currentAnimation.currentFrame / currentAnimation.frameLenght) * currentAnimation.frameSize.y
                };

                // Mirror sprite animation
                if (currentAnimation.isMirrored) {
                    sprite.spriteTexture.setTextureRect(sf::IntRect(frameCoord.x, frameCoord.y, -currentAnimation.frameSize.x, currentAnimation.frameSize.y));
                }
                else {
                    sprite.spriteTexture.setTextureRect(sf::IntRect(frameCoord.x, frameCoord.y, currentAnimation.frameSize.x, currentAnimation.frameSize.y));
                }
                
                currentAnimation.elapsedTime = 0.f;
            }
        }
    };
}