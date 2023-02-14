/**
 * @file SceneSerializer.h
 * @author flexhaufen
 * @brief Scene Serializer
 * @version 0.1
 * @date 2023-02-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Scene.h"



//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Scene Serialisation class
     * 
     */
    class SceneSerializer {

    public:

        /**
         * @brief Construct a new Scene Serializer object
         * 
         * @param scene     ref scene to serialize
         */
        SceneSerializer(const Ref<Scene>& scene);

        /**
         * @brief Serializes scene to given file
         * 
         * @param filepath  file save path
         */
        void Serialize(const std::string& filepath);

        // TODO
        void SerializeRuntime(const std::string& filepath);

        /**
         * @brief Deserializes scene from given file
         * 
         * @param filepath  file path
         * @return b8       true: success
         */
        b8 Deserialize(const std::string& filepath);

        // TODO
        b8 DeserializeRuntime(const std::string& filepath);

        
    private:

        /**
         * @brief Serializes given entity
         * 
         * @param out       YAML Emitter
         * @param entity    entity to serialize
         */
        void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity);

        /**
         * @brief Deserializes given entity
         * 
         * @param entity    YAML Entity itterator
         */
        void SceneSerializer::DeserializeEntity(YAML::detail::iterator_value& entity);

    private:

        // ** Members **
        Ref<Scene> m_Scene; // Active scene

    };
}