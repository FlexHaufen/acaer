/**
 * @file TextureHandler.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-07-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Entity/Components.h"

//*** DEFINES ***
#define AC_ERROR_TEXTURE_PATH   "assets/Textures/Debug/error_texture.png"


//*** NAMESPACE ***
namespace Acaer {

    class TextureHandler {
    public:

        /**
         * @brief Load a texture and associate it to given sprite component
         * 
         * @param sprite    Sprite Component
         * @param tag       Tag Component
         */
        void loadTexture(Component::Sprite &sprite, const Component::Tag &tag);

        /**
         * @brief Release a texture from an entity
         * 
         * @param sprite 
         */
        void releaseTexture(Component::Sprite &sprite);

    private:

        /**
         * @brief Load new Texture form file
         * 
         * @param filePath      file path
         * @param tag           Tag Component
         * @return sf::Texture
         */
        sf::Texture loadNewTexture(const std::string &filePath, const Component::Tag &tag) const;

    private:
        // ** Members **
        std::unordered_map<std::string, sf::Texture> m_Textures;       // Texture pool
        std::unordered_map<sf::Texture*, s32> m_TextureUsageCount;     // Usage count of given texture in pool

    };

}