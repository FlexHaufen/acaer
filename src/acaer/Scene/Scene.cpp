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

    Scene::Scene(sf::RenderWindow &window, EventManager &eventManager) : 
    m_Window(window),
    m_EventManager(eventManager) {

        AC_CORE_INFO("Initializing Scene");


        AC_CORE_INFO("Setting up Camera:");
        AC_CORE_INFO("    Width:  {0}", AC_WINDOW_X);
        AC_CORE_INFO("    Height: {0}", AC_WINDOW_Y);

        AC_CORE_INFO("Setting up Renderer");
        m_Renderer = CreateRef<Renderer>(window);
        m_DebugRenderer = CreateRef<DebugRenderer>(window);
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

    void Scene::DestroyEntity(Entity &entity) {
        // Destory Box2d Body
        if (entity.HasComponent<Component::RigidBody>()) {
            auto &rb = entity.GetComponent<Component::RigidBody>();
            m_PhysicsWorld->DestroyBody(rb.RuntimeBody);
        }
        // Destroy Sprite
        if (entity.HasComponent<Component::Sprite>()) {
            auto &sprite = entity.GetComponent<Component::Sprite>();
            m_SpriteHandler.DeleteSprite(sprite);
        }

        // Destory Body from registry
        m_Registry.destroy(entity);
    }

    void Scene::OnStart() {
        AC_PROFILE_FUNCTION();

        // ** Phyisics World **
        AC_CORE_INFO("Setting up PhysicsWorld");
        m_PhysicsWorld = CreateRef<b2World>(b2Vec2(AC_GRAVITY_X, AC_GRAVITY_Y));

        // ContactListener
        m_PhysicsWorld->SetContactListener(&m_ContactListener);

        // Setup box2d bodies
        m_Registry.view<Component::Transform, Component::RigidBody, Component::Collider>().each([&]( auto e, auto &transform, auto &rigidBody, auto &collider) {
            Convert::create_b2Body(rigidBody, transform, collider, m_PhysicsWorld.get());
        });

        #ifdef AC_DEBUG_RENDER
            AC_CORE_INFO("Setting up Debug Renderer");
            m_PhysicsWorld->SetDebugDraw(m_DebugRenderer.get());
        #endif

        // ** Sand World **
        AC_CORE_INFO("Setting up World");
        m_SandWorld = CreateRef<SandWorld>();

        // ** SpriteHandler **
        AC_CORE_INFO("Setting up SpriteHandler");
        m_Registry.view<Component::Sprite, Component::Tag>().each([&]( auto e, auto &sprite, auto &tag) {
            m_SpriteHandler.CreateSprite(sprite, tag);
        });

        // TODO (flex): Add NativeScript::OnStart() here

    }

    void Scene::OnEnd() {
        AC_PROFILE_FUNCTION();
    }

    void Scene::OnUpdate(f32 dt) {
        AC_PROFILE_FUNCTION();

        // ** Update Scripts **
        m_Registry.view<Component::NativeScript>().each([&](auto entity, auto& nsc) {
            if (!nsc.Instance) {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = Entity{ entity, this };
                nsc.Instance->OnCreate();
            }
            nsc.Instance->OnUpdate(dt);
        });


        // ** Physics **
        m_PhysicsWorld->Step(dt, AC_PHYSICS_VEL_STEPS, AC_PHYSICS_POS_STEPS);
        //m_SandWorld->OnUpdate();

        // retrive transform form box2d
        m_Registry.view<Component::Transform, Component::RigidBody, Component::Collider>().each([&]( auto e, auto &transform, auto &rigidBody, auto &collider) {
            b2Body* body = (b2Body*)rigidBody.RuntimeBody;

            // Calculate pos and rotation based on fixture
            transform.pos       = Convert::getPositionFrom_b2Body(body, collider);
            transform.rotation  = Convert::getRotationFrom_b2Body(body);
        });

        // ** Update Camera **
        if (m_useFreeCamera) {
            // TODO (flex): Add freecam here
        }
        else {
            m_Registry.view<Component::CameraController, Component::Transform>().each([&]( auto e, auto &cam, auto &transform) {
                m_Camera.OnUpdate(m_Window, transform.pos, cam.zoom, dt);
            });
        }

        // ** Update Sprites **
        m_Registry.view<Component::Tag, Component::Transform, Component::Sprite>().each([&]( auto e, auto &tag, auto &transform, auto &sprite) {
            Entity entity = {e, this};

            //AC_CORE_TRACE("animate {0}", tag.tag);

            // Update Dynamic Sprites
            if (entity.HasComponent<Component::SpriteAnimatior>()) {
                auto &sa = entity.GetComponent<Component::SpriteAnimatior>();
                m_SpriteHandler.HandleDynamicSprite(dt, sprite, sa, transform);
            }
            // Update Static Sprites
            else {
                m_SpriteHandler.HandleStaticSprite(sprite, transform);
            }
        });
    }

    void Scene::OnRender() {
        AC_PROFILE_FUNCTION();

        // ** Render **

        // Render layers
        for (u8 i = 0; i <= AC_MAX_RENDERLAYERS; i++) {
            m_Registry.view<Component::Tag, Component::Transform>().each([&]( auto e, auto &tag, auto &transform) {
                if (transform.renderLayer != i) {
                    return;
                }

                Entity entity = {e, this};
                #ifdef AC_DEBUG_RENDER
                    m_DebugRenderer->RenderMouseCoords(m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window)));
                    m_DebugRenderer->RenderTransformOrigin(transform);
                    m_DebugRenderer->RenderTag(tag, transform);
                #endif
                if (entity.HasComponent<Component::Sprite>()) {
                    auto &s = entity.GetComponent<Component::Sprite>();
                    
                    m_Renderer->RenderSprite(s);

                    #ifdef AC_DEBUG_RENDER
                        m_DebugRenderer->RenderSpriteOutline(transform, s);
                    #endif
                }
            });
        }

        
        for (size_t x = 0; x < SAND_WORLD_SIZE_X;  x++) {
            for (size_t y = 0; y < SAND_WORLD_SIZE_Y; y++) {
                if (m_SandWorld->GetCell(x, y).type != CellType::EMPTY) {
                    m_Renderer->RenderCell(x, y, m_SandWorld->GetCell(x, y).color);
                }
            }
        }
        

        #ifdef AC_DEBUG_RENDER
            m_PhysicsWorld->DebugDraw();
        #endif
    }
}