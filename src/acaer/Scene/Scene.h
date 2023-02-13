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
        Entity CreateEntity(std::string tag = "entity");

        /**
         * @brief Update function
         * 
         * @param dt delta time
         */
        void OnUpdate(f32 dt);

    private:

        /**
         * @brief Handels Input Components
         * 
         */
        void HandleInput_C();

        /**
         * @brief Handles Camera Component
         * 
         */
        void HandleCamera_C();

        /**
         * @brief Main render update function
         * 
         */
        void Render();

        /**
         * @brief Renders given transform
         * 
         * @param transform transform to render
         */
        void RenderTransform(Transform_C &transform);


        // ** Members **
        entt::registry m_Registry;      // entt Registry

        Camera2D m_Camera;              // Raylib Camera
        
        friend class Entity;            // Entity class
        friend class SceneSerializer;   // Scene Serializer
    };
}