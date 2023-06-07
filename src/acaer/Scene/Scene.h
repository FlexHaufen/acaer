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
#include "acaer/Scene/Entity/Components.h"

#include "acaer/Scene/World/World.h"

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


        void OnStart();

        void OnEnd();

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
         * @brief Destroys given entity
         * 
         * @param entity 
         */
        void DestroyEntity(Entity entity);

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


    private:
        
        /**
         * @brief Renders given transform
         * 
         * @param transform transform to render
         * @param tag       tag component
         */
        void RenderTransform(Component::Transform &transform, Component::Tag &tag);


        // ** Members **
        entt::registry m_Registry;      // entt Registry

        sf::View m_Camera;              // Camera

        b2World* m_PhysicsWorld = nullptr;
        World* m_World = nullptr;


        friend class Entity;            // Entity class
        friend class SceneSerializer;   // Scene Serializer
    
        friend class EntityBrowserPanel;
    };
}