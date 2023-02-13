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


    class SceneSerializer {

    public:
        SceneSerializer(const Ref<Scene>& scene);


        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);


        b8 Deserialize(const std::string& filepath);
        b8 DeserializeRuntime(const std::string& filepath);

        
    private:
        void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity);
        void SceneSerializer::DeserializeEntity(YAML::detail::iterator_value& entity);

    private:
        // ** Members **

        Ref<Scene> m_Scene;

    };


};