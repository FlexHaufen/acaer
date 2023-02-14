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



// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    Scene::Scene() {

        m_Camera.offset = {AC_WINDOW_X / 2.0f, AC_WINDOW_Y / 2.0f};
        m_Camera.zoom = 1.0f;
        m_Camera.rotation = 0.0f;
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



    void Scene::OnUpdate(f32 dt) {

        // Input
        HandleInput_C(dt);

        // Update
        HandleCamera_C();
    }


    void Scene::HandleInput_C(f32 dt) {
        auto group = m_Registry.group<Input_C, Transform_C>();
        for (auto entity : group) {
            auto &transform = group.get<Transform_C>(entity);
            auto &input = group.get<Input_C>(entity);

            // Basic player moevment
            if (input.isControllable) {
                // TODO: player speed const
                if (IsKeyDown(KEY_W)) {transform.rec.y -= 200.f * dt;}
                if (IsKeyDown(KEY_S)) {transform.rec.y += 200.f * dt;}
                if (IsKeyDown(KEY_A)) {transform.rec.x -= 200.f * dt;}
                if (IsKeyDown(KEY_D)) {transform.rec.x += 200.f * dt;}
            }
        }
    }

    void Scene::HandleCamera_C() {
        // Update camera
        auto group = m_Registry.group<Camera_C>(entt::get<Transform_C>);
        for (auto entity : group) {
            auto &transform = group.get<Transform_C>(entity);

            m_Camera.target = {transform.rec.x, transform.rec.y};
        }
    }

    void Scene::OnRender() {
        BeginMode2D(m_Camera);
            // ** Render **
            {
                auto group = m_Registry.group<Tag_C>(entt::get<Transform_C>);
                for (auto entity : group) {
                    auto &transform = group.get<Transform_C>(entity);
                    auto &tag = group.get<Tag_C>(entity);
                    RenderTransform(transform, tag);
                }
            }
        EndMode2D();
        // Render GUI heres
    }

    void Scene::RenderTransform(Transform_C &transform, Tag_C &tag) {
        #ifdef AC_RENDER_ENTITY_HITBOX
            DrawRectangleLines(int(transform.rec.x),
                                int(transform.rec.y),
                                int(transform.rec.width),
                                int(transform.rec.height),
                                transform.color);
        #endif

        #ifdef AC_RENDER_ENTITY_REC
            DrawRectangle(int(transform.rec.x),
                                int(transform.rec.y),
                                int(transform.rec.width),
                                int(transform.rec.height),
                                transform.color);
        #endif

        #ifdef AC_RENDER_ENTITY_TAG
                DrawText(tag.tag.c_str(), 
                int(transform.rec.x), 
                int(transform.rec.y - 20),          // little offset so the tag will display above rec
                AC_RENDER_ENTITY_TAG_FONT_SIZE, 
                AC_RENDER_ENTITY_TAG_FONT_COLOR);
        #endif

        #ifdef AC_RENDER_ENTITY_UUID
                DrawText(std::to_string(tag.uuid).c_str(), 
                int(transform.rec.x), 
                int(transform.rec.y - 40),           // little offset so the uuid will display above rec
                AC_RENDER_ENTITY_UUID_FONT_SIZE, 
                AC_RENDER_ENTITY_UUID_FONT_COLOR);
        #endif
    }
}