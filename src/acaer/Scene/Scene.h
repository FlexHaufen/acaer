/**
 * @file Scene.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-02-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
# pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Components.h"

//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {
    class Entity;

    /**
     * @brief World Scene
     * 
     */
    class Scene {

    public:
        Scene();
        ~Scene();

        static Ref<Scene> Copy(Ref<Scene> other);

        /**
         * @brief Create a Entity object
         * 
         * @param tag       tag (name) of entity
         * @return Entity 
         */
        Entity CreateEntity(const std::string& name = "entity");

        /**
         * @brief Create a Entity with a uuid
         * 
         * @param uuid      given uuid
         * @param name      given name
         * @return Entity   created entity
         */
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = "entity");

        /**
         * @brief Update function
         * 
         * @param dt delta time
         */
        void OnUpdate(f32 dt);

        /**
         * @brief Main render update function
         * 
         */
        void OnRender();

    private:

        /**
         * @brief Handels Input Components
         * 
         * @param dt delta time
         */
        void HandleInput_C(f32 dt);

        /**
         * @brief Handles Camera Component
         * 
         */
        void HandleCamera_C();

        

        /**
         * @brief Renders given transform
         * 
         * @param transform transform to render
         * @param tag       tag component
         */
        void RenderTransform(Transform_C &transform, Tag_C &tag);


        // ** Members **
        entt::registry m_Registry;      // entt Registry

        Camera2D m_Camera;              // Raylib Camera
        
        friend class Entity;            // Entity class
        friend class SceneSerializer;   // Scene Serializer
    };
}