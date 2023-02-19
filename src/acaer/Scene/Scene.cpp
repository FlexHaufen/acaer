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


    void Scene::OnStart() {
        m_PhysicsWorld = new b2World({ 0.0f, 10.f});

        auto view = m_Registry.view<RigidBody_C>();
        for (auto e: view) {
            Entity entity = {e, this};

            auto& t = entity.GetComponent<Transform_C>();
            auto& rb = entity.GetComponent<RigidBody_C>();

            b2BodyDef bodyDef;

            bodyDef.type = (b2BodyType)rb.type;     // NOTE: Type conversion is possible because of same order
            bodyDef.position.Set(t.pos.x / AC_PPM, t.pos.y / AC_PPM);
            bodyDef.angle = t.rotation / AC_DEG_PER_RAD;

            b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
            body->SetFixedRotation(rb.fixedRoation);
            rb.RuntimeBody = body;


            b2PolygonShape polyShape;
            // TODO: Add ability to don't use scale 
            polyShape.SetAsBox((t.size.x / 2.f / AC_PPM), (t.size.y / 2.f / AC_PPM));
            b2FixtureDef fixtureDef;
            fixtureDef.shape        = &polyShape;
            fixtureDef.density      = rb.density;
            fixtureDef.restitution  = rb.restitution;
            fixtureDef.friction     = rb.friction;
            fixtureDef.restitutionThreshold = rb.restitutionThreshold;
            body->CreateFixture(&fixtureDef);
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
                static const f32 speed = 5;
               
                // ----- Smoothening in x & y
                sf::Vector2f movement = sf::Vector2f(t.pos.x, t.pos.y) - m_Camera.getCenter();
                m_Camera.move(movement * dt * speed);
                
                // ----- No smoothening
                //m_Camera.setCenter(sf::Vector2(t.pos.x, t.pos.y));
                

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
                auto& t = entity.GetComponent<Transform_C>();
                auto& rb = entity.GetComponent<RigidBody_C>();

                b2Body* body = (b2Body*)rb.RuntimeBody;
                t.pos       = {body->GetPosition().x * AC_PPM, body->GetPosition().y * AC_PPM};
                t.rotation  =  body->GetAngle() * AC_DEG_PER_RAD * -1;
            }
        }
    }


    void Scene::OnRender(sf::RenderWindow &window) {
        // ** Render **
        {
            auto group = m_Registry.group<Tag_C>(entt::get<Transform_C, RigidBody_C>);
            for (auto entity : group) {
                auto &t = group.get<Transform_C>(entity);
                auto &tag = group.get<Tag_C>(entity);
                auto &rb = group.get<RigidBody_C>(entity);

                sf::RectangleShape rec;
                rec.setPosition(sf::Vector2f(t.pos.x, t.pos.y));
                rec.setRotation(t.rotation);
                rec.setOrigin(sf::Vector2f(t.size.x / 2.f, t.size.y / 2.f));
                rec.setSize(sf::Vector2f(t.size.x, t.size.y));

                rec.setFillColor(sf::Color(0, 0, 0 , 0));       // Setting the fillcolor to nothing
                rec.setOutlineThickness(AC_RENDER_ENTITY_HITBOX_THICKNESS);
                rec.setOutlineColor(sf::Color(t.color.r, t.color.a, t.color.b, t.color.a));
                window.draw(rec);

                // Render origin
                static const f32 radius = 2.f;
                sf::CircleShape c;
                c.setRadius(radius);
                c.setFillColor(sf::Color::Red);
                c.setPosition(sf::Vector2f(t.pos.x - radius, t.pos.y - radius));
                window.draw(c);
            }

        }
    }

}