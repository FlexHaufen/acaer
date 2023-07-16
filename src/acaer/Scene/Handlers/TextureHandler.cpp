/**
 * @file TextureHandler.cpp
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-07-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Scene/Handlers/TextureHandler.h"


// *** NAMESPACE ***
namespace Acaer {

    void TextureHandler::loadTexture(Component::Sprite &sprite, const Component::Tag &tag) {
            
        // Check if givent texture exists
        if (m_Textures.find(sprite.texturepath) != m_Textures.end()) {
            m_TextureUsageCount[&m_Textures[sprite.texturepath]]++;         // Increase usage count for the loaded texture
        }
        else {
            m_Textures.insert(std::make_pair(sprite.texturepath, loadNewTexture(sprite.texturepath, tag))); // Add Textures to pool
            m_TextureUsageCount.insert(std::make_pair(&m_Textures[sprite.texturepath], 1));                 // Add Usage Count
        }
        sprite.spriteTexture.setTexture(m_Textures[sprite.texturepath]);    // set sprite texture to pool reference
    }

    void TextureHandler::releaseTexture(Component::Sprite &sprite) {

        // Decrease the usage count for the released texture
        m_TextureUsageCount[&m_Textures[sprite.texturepath]]--;

        // If no more entities are using the texture, remove it from the registry
        if (m_TextureUsageCount[&m_Textures[sprite.texturepath]] == 0) {
            m_TextureUsageCount.erase(&m_Textures[sprite.texturepath]);
            m_Textures.erase(sprite.texturepath);
        }
    }

    sf::Texture TextureHandler::loadNewTexture(const std::string &filePath, const Component::Tag &tag) const {
        sf::Texture texture;
        if (!texture.loadFromFile(filePath)) {
            AC_CORE_WARN("Renderer could not find valid texture of Entity:");
            AC_CORE_WARN("    - {0}: [{1}]", tag.tag, tag.uuid);

            if (!texture.loadFromFile(AC_ERROR_TEXTURE_PATH)) {
                AC_CORE_ERROR("Failed to load error_texture");
            }
        }
        return texture;
    }

}