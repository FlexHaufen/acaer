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
#define AC_RENDER_COLLIDERS

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
		auto& tag = entity.AddComponent<Component::Tag>();
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

        auto view = m_Registry.view<Component::RigidBody>();
        for (auto e: view) {
            Entity entity = {e, this};

            // Create Physics Body
            if (entity.HasComponent<Component::RigidBody>()) {
                auto &t = entity.GetComponent<Component::Transform>();
                auto &rb = entity.GetComponent<Component::RigidBody>();
                auto &c = entity.GetComponent<Component::Collider>();
                Convert::create_b2Body(rb, t, c, m_PhysicsWorld);
            }
        }

        //!------- DEBUG --------
        /*
        {
            Cell c;
            c.type  = CellType::SAND;
            c.props = CellProperties::MOVE_DOWN | CellProperties::MOVE_DOWN_SIDE;
            //c.props = CellProperties::NONE;

            c.color = {255, 0, 255, 255};       // pink

            for (int x = 30; x <= 49; x++) {
                for (int y = -30; y <= 0; y++) {
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
        */
        //!---------------------
    }

    void Scene::OnEnd() {
        delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;

        delete m_World;
        m_World = nullptr;
    }


    void Scene::OnUpdate(f32 dt, sf::RenderWindow &window) {

/*
        {
            Cell c;
            c.type  = CellType::SAND;
            //c.props = CellProperties::MOVE_DOWN | CellProperties::MOVE_DOWN_SIDE;
            c.props = CellProperties::NONE;
            c.color = {0, 255, 255, 255};       // blue

            // FIXME: still problems on down movement
            //if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            //    sf::Vector2i mPos = sf::Mouse::getPosition() / sf::Vector2i(AC_GLOBAL_SCALE, AC_GLOBAL_SCALE) - window.getSize() / 2;
            //    m_World->SetCell(mPos.x, mPos.y / AC_GLOBAL_SCALE, c);
            //}
        }
*/


        // ** Update Scripts **
        m_Registry.view<Component::NativeScript>().each([=](auto entity, auto& nsc) {
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
            auto view = m_Registry.view<Component::RigidBody>();
            for (auto e : view) {
                Entity entity = {e , this};
                auto& tag = entity.GetComponent<Component::Tag>();
                auto& t = entity.GetComponent<Component::Transform>();
                auto& rb = entity.GetComponent<Component::RigidBody>();
                auto& c = entity.GetComponent<Component::Collider>();

                b2Body* body = (b2Body*)rb.RuntimeBody;

                // Calculate pos and rotation based on fixture
                t.pos       = Convert::getPositionFrom_b2Body(body, c);
                t.rotation  = Convert::getRotationFrom_b2Body(body);
            }
        }

        // ** Update Camera **
        {
            auto group = m_Registry.group<Component::Camera>(entt::get<Component::Transform>);
            for (auto entity : group) {
                auto &t = group.get<Component::Transform>(entity);
                auto &cam = group.get<Component::Camera>(entity);
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
            auto group = m_Registry.group<Component::Tag>(entt::get<Component::Transform>);
            for (auto e : group) {

                Entity entity = {e, this};

                auto &tag = entity.GetComponent<Component::Tag>();
                auto &t = entity.GetComponent<Component::Transform>();
               
                // render Sprite
                if (entity.HasComponent<Component::Sprite>()) {
                    auto &s = entity.GetComponent<Component::Sprite>();
                    Renderer::RenderSprite(window, t, s, true, true);
                }

                // Render Colliders
                #ifdef AC_RENDER_COLLIDERS
                if (entity.HasComponent<Component::Collider>()) {
                    auto &c = entity.GetComponent<Component::Collider>();
                    Renderer::RenderHitbox(window, t, c);
                    Renderer::RenderSensor(window, t, c);
                }
                #endif
            }
        }
        m_World->OnRender(window);
    }
}