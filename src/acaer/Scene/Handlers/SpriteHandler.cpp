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

        void SpriteHandler::OnStart(Component::Sprite &sprite, const Component::Tag &tag) {
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

        void SpriteHandler::HandleStaticSprite(Component::Sprite &sprite, const Component::Transform &transform) {
            sprite.spriteTexture.setPosition(sf::Vector2f(transform.pos.x, transform.pos.y));
            sprite.spriteTexture.setRotation(transform.rotation);
        
        }

        void SpriteHandler::HandleDynamicSprite(f32 dt, Component::Sprite &sprite, Component::SpriteAnimatior &spriteAnimator, const Component::Transform &transform) {
            m_AnimationHandler.OnUpdate(dt, sprite, spriteAnimator);
            sprite.spriteTexture.setRotation(transform.rotation);
            sprite.spriteTexture.setPosition(sf::Vector2f(transform.pos.x, transform.pos.y));
        }

}