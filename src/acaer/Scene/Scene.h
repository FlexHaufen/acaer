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
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Entity/Components.h"
#include "acaer/Scene/ContactListener/ContactListener.h"

#include "acaer/Scene/Renderer/Renderer.h"
#include "acaer/Scene/Renderer/DebugRenderer.h"

#include "acaer/Scene/Handlers/SpriteHandler.h"

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
        Scene(sf::RenderWindow &window);
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
         */
        void OnUpdate(f32 dt);

        /**
         * @brief Main render update function
         * 
         * @param dt delta time
         */
        void OnRender(f32 dt);


    private:

        // ** Members **
        sf::View m_Camera;                          // Camera
        sf::RenderWindow &m_Window;                 // Ref to sf::RenderWindow
        Renderer*       m_Renderer      = nullptr;  // Renderer
        DebugRenderer*  m_DebugRenderer = nullptr;  // DebugRenderer
        SpriteHandler   m_SpriteHandler;            // SpriteHandler
        
        entt::registry  m_Registry;                 // entt Registry
        b2World*        m_PhysicsWorld  = nullptr;  // Simulated Physics World (for RigidBodies)
        SandWorld*      m_SandWorld     = nullptr;  // Simulated Sand World (for Pixelsimulation)

        ContactListener m_ContactListener;          // Box2D contactlistener

        friend class Entity;                        // Entity class
        friend class SceneSerializer;               // Scene Serializer

        // * ImGui *
        friend class EntityBrowserPanel;            // ImGui Panel
    };
}