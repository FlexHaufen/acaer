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
         * @param window sf::Window reference
         */
        void OnUpdate(f32 dt, sf::RenderWindow &window);

        /**
         * @brief Main render update function
         * 
         * @param window sf::Window reference
         */
        void OnRender(sf::RenderWindow &window);

        /**
         * @brief Handle inputs
         * 
         * @param dt delta time
         * @param e event
         */
        void OnInput(f32 dt, sf::Event &e);

    private:
        
        /**
         * @brief Renders given transform
         * 
         * @param transform transform to render
         * @param tag       tag component
         */
        void RenderTransform(Transform_C &transform, Tag_C &tag);


        // ** Members **
        entt::registry m_Registry;      // entt Registry

        sf::View m_Camera;              // Camera

        friend class Entity;            // Entity class
        friend class SceneSerializer;   // Scene Serializer
    };
}