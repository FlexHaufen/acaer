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
#include "acaer/Scene/Entity.h"
#include "acaer/Scene/ScriptableEntity.h"

#include "acaer/Scene/Renderer/Renderer.h"

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

        auto view = m_Registry.view<RigidBody_C>();
        for (auto e: view) {
            Entity entity = {e, this};

            // Create Physics Body
            if (entity.HasComponent<RigidBody_C>()) {
                auto &t = entity.GetComponent<Transform_C>();
                auto &rb = entity.GetComponent<RigidBody_C>();
                auto &c = entity.GetComponent<Collider_C>();


                b2BodyDef bodyDef;

                bodyDef.type = (b2BodyType)rb.type;     // NOTE: Type conversion is possible because of same order
                bodyDef.position.Set(t.pos.x / AC_PPM, t.pos.y / AC_PPM);
                bodyDef.angle = t.rotation / AC_DEG_PER_RAD;

                b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
                body->SetFixedRotation(rb.fixedRoation);
                rb.RuntimeBody = body;


                b2PolygonShape polyShape;
                // TODO: Add ability to don't use scale 
                polyShape.SetAsBox((c.size.x / 2.f / AC_PPM), (c.size.y / 2.f / AC_PPM));
                b2FixtureDef fixtureDef;
                fixtureDef.shape        = &polyShape;
                fixtureDef.density      = rb.density * AC_PPM;
                fixtureDef.restitution  = rb.restitution;
                fixtureDef.friction     = rb.friction;
                fixtureDef.restitutionThreshold = rb.restitutionThreshold;
                body->CreateFixture(&fixtureDef);
            }
        }
    }

    void Scene::OnEnd() {
        delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
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

        // ** Update Camera **
        {
            auto group = m_Registry.group<Camera_C>(entt::get<Transform_C>);
            for (auto entity : group) {
                auto &t = group.get<Transform_C>(entity);
                auto &cam = group.get<Camera_C>(entity);
                static const f32 speed = 5;
                // ----- Smoothening in x & y
                sf::Vector2f movement = sf::Vector2f(t.pos.x, t.pos.y) - m_Camera.getCenter();
                m_Camera.move(movement * dt * speed);
                
                // ----- No smoothening
                //m_Camera.setCenter(sf::Vector2(t.pos.x, t.pos.y));
                
                m_Camera.setSize(sf::Vector2f(window.getSize().x * cam.zoom, window.getSize().y * cam.zoom));


                window.setView(m_Camera);
            }
        }

        // ** Physics **
        {
            m_PhysicsWorld->Step(dt, AC_PHYSICS_VEL_STEPS, AC_PHYSICS_POS_STEPS);

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
                t.pos       = {(body->GetPosition().x * AC_PPM) - c.size.x / 2 + c.offset.x , (body->GetPosition().y * AC_PPM) - c.size.y / 2 + c.offset.y};
                t.rotation  =  body->GetAngle() * AC_DEG_PER_RAD * -1;
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
    }
}