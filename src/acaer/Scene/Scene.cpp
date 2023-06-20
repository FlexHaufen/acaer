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


#include "acaer/Helper/Convert/Convert.h"

// *** DEFINE ***
#define AC_DEBUG_RENDER

// *** NAMESPACE ***
namespace Acaer {

    // FIXME (flex): Dont init debugrenderer in constructor
    Scene::Scene(sf::RenderWindow &window) : 
    m_Window(window) {
        AC_CORE_INFO("Initializing Scene");


        AC_CORE_INFO("Setting up Camera:");
        AC_CORE_INFO("    Width:  {0}", AC_WINDOW_X);
        AC_CORE_INFO("    Height: {0}", AC_WINDOW_Y);
        m_Camera.setSize(sf::Vector2f(AC_WINDOW_X, AC_WINDOW_Y));

        AC_CORE_INFO("Setting up Renderer");
        m_Renderer = new Renderer(window);
        m_DebugRenderer = new DebugRenderer(window);
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
        AC_PROFILE_FUNCTION();

        AC_CORE_INFO("Setting up PhysicsWorld");
        m_PhysicsWorld = new b2World({AC_GRAVITY_X, AC_GRAVITY_Y});

        #ifdef AC_DEBUG_RENDER
            AC_CORE_INFO("Setting up Debug Renderer");
            m_PhysicsWorld->SetDebugDraw(m_DebugRenderer);
        #endif

        AC_CORE_INFO("Setting up World");
        m_World = new World(AC_WORLD_CHUNCK_SIZE, AC_WORLD_CHUNCK_SIZE, 1);

        // ** ContactListener **
        m_PhysicsWorld->SetContactListener(&m_ContactListener);
        {
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
        }

        AC_CORE_INFO("Setting up SpriteHandler");
        {
            auto view = m_Registry.view<Component::Sprite>();
            for (auto e: view) {
                Entity entity = {e, this};

                // Create Physics Body
                if (entity.HasComponent<Component::Sprite>()) {
                    auto &s = entity.GetComponent<Component::Sprite>();
                    auto &t = entity.GetComponent<Component::Tag>();
                    m_SpriteHandler.OnStart(s, t);
                }
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
        AC_PROFILE_FUNCTION();

        // FIXME (flex): make shared pointer
        delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;

        delete m_World;
        m_World = nullptr;

        delete m_Renderer;
        m_Renderer = nullptr;
    
        delete m_DebugRenderer;
        m_DebugRenderer = nullptr;
    }


    void Scene::OnUpdate(f32 dt) {
        AC_PROFILE_FUNCTION();

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
                
                m_Camera.setSize(sf::Vector2f(m_Window.getSize().x * cam.zoom, m_Window.getSize().y * cam.zoom));
                m_Window.setView(m_Camera);
            }
        }

        // ** Update Sprites **
        {
            auto view = m_Registry.view<Component::Sprite>();
            for (auto e : view) {
                Entity entity = {e, this};
                auto &s = entity.GetComponent<Component::Sprite>();
                auto &t = entity.GetComponent<Component::Transform>();

                // Update Dynamic Sprites
                if (entity.HasComponent<Component::SpriteAnimatior>()) {
                    auto &sa = entity.GetComponent<Component::SpriteAnimatior>();
                    m_SpriteHandler.HandleDynamicSprite(dt, s, sa, t);
                }
                // Update Static Sprites
                else {
                    m_SpriteHandler.HandleStaticSprite(s, t);
                }
            }
        }
    }


    void Scene::OnRender(f32 dt) {
        AC_PROFILE_FUNCTION();

        // ** Render **
        {
            auto group = m_Registry.group<Component::Tag>(entt::get<Component::Transform>);
            for (auto e : group) {

                Entity entity = {e, this};

                auto &tag = entity.GetComponent<Component::Tag>();
                auto &t = entity.GetComponent<Component::Transform>();
               
                #ifdef AC_DEBUG_RENDER
                    m_DebugRenderer->RenderTransformOrigin(t);
                #endif

                if (entity.HasComponent<Component::Sprite>()) {
                    auto &s = entity.GetComponent<Component::Sprite>();
                   
                    m_Renderer->RenderSprite(s);

                    #ifdef AC_DEBUG_RENDER
                        m_DebugRenderer->RenderSpriteOutline(t, s);
                    #endif

                }
            }
        }
        //m_World->OnRender(m_Window);

        #ifdef AC_DEBUG_RENDER
            m_PhysicsWorld->DebugDraw();
        #endif
    }
}