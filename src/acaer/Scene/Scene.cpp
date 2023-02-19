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
        m_Camera.setSize(sf::Vector2f(AC_WINDOW_X, AC_WINDOW_Y));
    }

    Scene::~Scene() {

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


    void Scene::OnInput(f32 dt, sf::Event &e) {
        // Input
        {
            auto group = m_Registry.group<Input_C, Transform_C>();
            for (auto entity : group) {
                auto &transform = group.get<Transform_C>(entity);
                auto &input = group.get<Input_C>(entity);

                // Basic player moevment
                if (input.isControllable) {
                    // "close requested" event: we close the window
                    if (e.key.code == sf::Keyboard::W) {transform.pos.y -= 200.f * dt * 100;}
                    if (e.key.code == sf::Keyboard::S) {transform.pos.y += 200.f * dt * 100;}
                    if (e.key.code == sf::Keyboard::A) {transform.pos.x -= 200.f * dt * 100;}
                    if (e.key.code == sf::Keyboard::D) {transform.pos.x += 200.f * dt * 100;}
                }
            }
        }
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
    }


    void Scene::OnRender(sf::RenderWindow &window) {
        // ** Render **
        {
            auto group = m_Registry.group<Tag_C>(entt::get<Transform_C>);
            for (auto entity : group) {
                auto &t = group.get<Transform_C>(entity);
                auto &tag = group.get<Tag_C>(entity);
                sf::RectangleShape rec;
                rec.setSize(sf::Vector2f(t.size.x, t.size.y));
                rec.setPosition(sf::Vector2f(t.pos.x, t.pos.y));
                rec.setFillColor(sf::Color(0, 0, 0 , 0));       // Setting the fillcolor to nothing
                rec.setOutlineThickness(AC_RENDER_ENTITY_HITBOX_THICKNESS);
                rec.setOutlineColor(sf::Color(t.color.r, t.color.a, t.color.b, t.color.a));
                window.draw(rec);
            }
        }
    }
}