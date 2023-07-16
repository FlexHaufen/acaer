/**
 * @file SpriteHandler.cpp
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Scene/Handlers/SpriteHandler.h"


// *** NAMESPACE ***
namespace Acaer {

        SpriteHandler::SpriteHandler() {
            m_AnimationHandler = CreateRef<AnimationHandler>();
            m_TextureHandler = CreateRef<TextureHandler>();
        }

        void SpriteHandler::CreateSprite(Component::Sprite &sprite, const Component::Tag &tag) {
            m_TextureHandler->loadTexture(sprite, tag);
            sprite.spriteTexture.setScale({AC_GLOBAL_SCALE, AC_GLOBAL_SCALE});
        }

        void SpriteHandler::DeleteSprite(Component::Sprite &sprite) {
            m_TextureHandler->releaseTexture(sprite);
        }

        void SpriteHandler::HandleStaticSprite(Component::Sprite &sprite, const Component::Transform &transform) {
            sprite.spriteTexture.setPosition(sf::Vector2f(transform.pos.x, transform.pos.y));
            sprite.spriteTexture.setRotation(transform.rotation);
        
        }

        void SpriteHandler::HandleDynamicSprite(f32 dt, Component::Sprite &sprite, Component::SpriteAnimatior &spriteAnimator, const Component::Transform &transform) {
            m_AnimationHandler->OnUpdate(dt, sprite, spriteAnimator);
            sprite.spriteTexture.setRotation(transform.rotation);
            sprite.spriteTexture.setPosition(sf::Vector2f(transform.pos.x, transform.pos.y));
        }

}