/**
 * @file Scene.cpp
 * @author flaxhaufen
 * @brief 
 * @version 0.1
 * @date 2023-02-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Scene/Scene.h"
#include "acaer/Scene/Entity/Entity.h"
#include "acaer/Scene/Entity/ScriptableEntity.h"

#include "acaer/Scene/Renderer/Renderer.h"

#include "acaer/Helper/Convert/Convert.h"

// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    Scene::Scene() {
        AC_CORE_INFO("Initializing Scene");

        AC_CORE_INFO("Setting up Camera:");
        AC_CORE_INFO("    Width:  {0}", AC_WINDOW_X);
        AC_CORE_INFO("    Height: {0}", AC_WINDOW_Y);
        m_Camera.setSize(sf::Vector2f(AC_WINDOW_X, AC_WINDOW_Y));
    }

    Scene::~Scene() {
        AC_CORE_INFO("Terminating Scene");
    }

    Entity Scene::CreateEntity(const std::string& name) {
        return CreateEntityWithUUID(UUID(), name);
	}

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name) {
        Entity entity = { m_Registry.create(), this };
		auto& tag = entity.AddComponent<Tag_C>();
        tag.uuid = uuid;
		tag.tag = name;

        // m_EntityMap[uuid] = entity;

		return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        // TODO (flex): Delete entity at runtime        
        //m_Registry.destroy(entity);
        
        AC_CORE_WARN("Can't delete entity at runtime...");
    }

    void Scene::OnStart() {
        m_PhysicsWorld = new b2World({AC_GRAVITY_X, AC_GRAVITY_Y});
        m_World = new World(AC_WORLD_CHUNCK_SIZE, AC_WORLD_CHUNCK_SIZE, 1);

        auto view = m_Registry.view<RigidBody_C>();
        for (auto e: view) {
            Entity entity = {e, this};

            // Create Physics Body
            if (entity.HasComponent<RigidBody_C>()) {
                auto &t = entity.GetComponent<Transform_C>();
                auto &rb = entity.GetComponent<RigidBody_C>();
                auto &c = entity.GetComponent<Collider_C>();

                Convert::create_b2Body(rb, t, c, m_PhysicsWorld);
            }
        }

        //!------- DEBUG --------
        {
            Cell c;
            c.type  = CellType::SAND;
            c.props = CellProperties::MOVE_DOWN | CellProperties::MOVE_DOWN_SIDE;
            //c.props = CellProperties::NONE;

            c.color = {255, 0, 255, 255};       // pink

            for (int x = 30; x <= 49; x++) {
                for (int y = 30; y <= 49; y++) {
                    m_World->SetCell(x, y, c);
                }
            }
        }
        {
            Cell c;
            c.type  = CellType::SAND;
            //c.props = CellProperties::MOVE_DOWN | CellProperties::MOVE_DOWN_SIDE;
            c.props = CellProperties::NONE;
             c.color = {0, 255, 255, 255};       // blue

            // FIXME: still problems on down movement
            for (int x = 0; x <= 49; x++) {
                m_World->SetCell(x, 69, c);
            }
        }
        //!---------------------
    }

    void Scene::OnEnd() {
        delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;

        delete m_World;
        m_World = nullptr;
    }


    void Scene::OnUpdate(f32 dt, sf::RenderWindow &window) {

        // ** Update Scripts **
        m_Registry.view<NativeScript_C>().each([=](auto entity, auto& nsc) {
            if (!nsc.Instance) {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = Entity{ entity, this };
                nsc.Instance->OnCreate();
            }
            nsc.Instance->OnUpdate(dt);
        });



        // ** Physics **
        {
            m_PhysicsWorld->Step(dt, AC_PHYSICS_VEL_STEPS, AC_PHYSICS_POS_STEPS);
            m_World->OnUpdate();

            // retrive transform form box2d
            auto view = m_Registry.view<RigidBody_C>();
            for (auto e : view) {
                Entity entity = {e , this};
                auto& tag = entity.GetComponent<Tag_C>();
                auto& t = entity.GetComponent<Transform_C>();
                auto& rb = entity.GetComponent<RigidBody_C>();
                auto& c = entity.GetComponent<Collider_C>();

                b2Body* body = (b2Body*)rb.RuntimeBody;

                // Calculate pos and rotation based on fixture
                t.pos       = Convert::getPositionFrom_b2Body(body, c);
                t.rotation  = Convert::getRotationFrom_b2Body(body);
            }
        }

        // ** Update Camera **
        {
            auto group = m_Registry.group<Camera_C>(entt::get<Transform_C>);
            for (auto entity : group) {
                auto &t = group.get<Transform_C>(entity);
                auto &cam = group.get<Camera_C>(entity);
                static const f32 speed = 5;
                // ----- Smoothening in x & y
                //sf::Vector2f movement = sf::Vector2f(t.pos.x, t.pos.y) - m_Camera.getCenter();
                //m_Camera.move(movement * dt * speed);
                
                // ----- No smoothening
                m_Camera.setCenter(sf::Vector2(t.pos.x, t.pos.y));
                
                m_Camera.setSize(sf::Vector2f(window.getSize().x * cam.zoom, window.getSize().y * cam.zoom));


                window.setView(m_Camera);
            }
        }
    }


    void Scene::OnRender(sf::RenderWindow &window) {
        // ** Render **
        {
            auto group = m_Registry.group<Tag_C>(entt::get<Transform_C>);
            for (auto e : group) {

                Entity entity = {e, this};

                auto &tag = entity.GetComponent<Tag_C>();
                auto &t = entity.GetComponent<Transform_C>();
               
                if (entity.HasComponent<Sprite_C>()) {
                    auto &s = entity.GetComponent<Sprite_C>();
                    Renderer::RenderSprite(window, t, s, true, true);
                }

                if (entity.HasComponent<Collider_C>()) {
                    auto &c = entity.GetComponent<Collider_C>();
                    Renderer::RenderHitbox(window, t, c);
                }
            }
        }
        m_World->OnRender(window);
    }
}