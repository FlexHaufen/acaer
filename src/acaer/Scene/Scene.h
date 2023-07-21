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
#include "acaer/Scene/Camera/Camera.h"
#include "acaer/Scene/Handlers/SpriteHandler.h"
#include "acaer/Scene/World/SandWorld.h"
#include "acaer/Core/Events/EventManager.h"


//*** DEFINES ***
#define AC_MAX_RENDERLAYERS     10

//*** NAMESPACE ***
namespace Acaer {
    class Entity;

    /**
     * @brief World Scene
     * 
     */
    class Scene {

    public:
        Scene(sf::RenderWindow &window, EventManager &eventManager);
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
        void DestroyEntity(Entity &entity);

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

        // ** Members **
        // cam
        Camera m_Camera;                            // Camera
        b8 m_useFreeCamera;                         // True: When camera shall be free            

        // events
        EventManager       &m_EventManager;         // Ref to EventManager

        // rendering
        sf::RenderWindow   &m_Window;               // Ref to sf::RenderWindow
        Ref<Renderer>       m_Renderer;             // Renderer
        Ref<DebugRenderer>  m_DebugRenderer;        // DebugRenderer
        SpriteHandler       m_SpriteHandler;        // SpriteHandler
        
        entt::registry      m_Registry;             // entt Registry
        Ref<b2World>        m_PhysicsWorld;         // Simulated Physics World (for RigidBodies)
        Ref<SandWorld>      m_SandWorld;            // Simulated Sand World (for Pixelsimulation)

        ContactListener     m_ContactListener;      // Box2D contactlistener

        friend class Entity;                        // Entity class
        friend class SceneSerializer;               // Scene Serializer


        // FIXME (flex): Get workaround "friend class"
        // * ImGui *
        friend class EntityBrowserPanel;            // ImGui Panel
        friend class SceneEditorPanel;              // ImGui Panel
        friend class SandWorldEditorPanel;          // ImGui Panel
    };
}