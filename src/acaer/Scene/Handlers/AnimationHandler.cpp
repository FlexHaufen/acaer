/**
 * @file AnimationHandler.cpp
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Scene/Handlers/AnimationHandler.h"


// *** NAMESPACE ***
namespace Acaer {

    void AnimationHandler::OnUpdate(f32 dt, Component::Sprite &sprite, Component::SpriteAnimatior &spriteAnimator) {

        static v2<s32> frameCoord;

        // Get currently active animation
        Component::SpriteAnimatior::Animation &currentAnimation = spriteAnimator.pool[spriteAnimator.currentAnimation];
        currentAnimation.elapsedTime += dt;
        if (currentAnimation.elapsedTime >= 1.0f / currentAnimation.animationSpeed) {

            currentAnimation.currentFrame++;
            
            if (currentAnimation.currentFrame >= currentAnimation.frameLenght) {
                currentAnimation.currentFrame = 0;
            }

            // Calculate pos of current frame
            frameCoord = {
                .x = currentAnimation.currentFrame * currentAnimation.frameSize.x,
                .y = currentAnimation.framePos     * currentAnimation.frameSize.y
            };

            // Mirror sprite animation
            if (currentAnimation.isMirrored) {
                sprite.spriteTexture.setTextureRect(sf::IntRect(frameCoord.x + currentAnimation.frameSize.x, 
                                                                frameCoord.y,
                                                                -currentAnimation.frameSize.x,
                                                                currentAnimation.frameSize.y));
            }
            else {
                sprite.spriteTexture.setTextureRect(sf::IntRect(frameCoord.x, 
                                                                frameCoord.y, 
                                                                currentAnimation.frameSize.x, 
                                                                currentAnimation.frameSize.y));
            }
            
            currentAnimation.elapsedTime = 0.f;
        }
    }
}