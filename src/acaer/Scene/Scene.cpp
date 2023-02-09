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

        entt::entity entity = m_Registry.create();

        m_Camera.offset = {AC_WINDOW_X / 2.0f, AC_WINDOW_Y / 2.0f};
        m_Camera.zoom = 1.0f;
        m_Camera.rotation = 0.0f;
    }

    Scene::~Scene() {

    }


    Entity Scene::CreateEntity(std::string tag) {
        Entity entity(m_Registry.create(), this);

        auto &transform = entity.AddComponent<Transform_C>();
        transform.tag = tag;

        return entity;
    }



    void Scene::OnUpdate(f32 dt) {

        // Input
        HandleInput();

        // Update
        UpdateCamera();
        Render();
    }


    void Scene::HandleInput() {
        auto group = m_Registry.group<Input_C, Transform_C>();
        for (auto entity : group) {
            auto &transform = group.get<Transform_C>(entity);
            auto &input = group.get<Input_C>(entity);

            // Basic player moevment
            if (input.isControllable) {
                if (IsKeyDown(KEY_W)) {transform.rec.y -= 0.5f;}
                if (IsKeyDown(KEY_S)) {transform.rec.y += 0.5f;}
                if (IsKeyDown(KEY_A)) {transform.rec.x -= 0.5f;}
                if (IsKeyDown(KEY_D)) {transform.rec.x += 0.5f;}
            }
        }
    }

    void Scene::UpdateCamera() {
        // Update camera
        auto group = m_Registry.group<Camera_C>(entt::get<Transform_C>);
        for (auto entity : group) {
            auto &transform = group.get<Transform_C>(entity);

            m_Camera.target = {transform.rec.x, transform.rec.y};
        }
    }

    void Scene::Render() {
        BeginDrawing();
            ClearBackground(AC_SCENE_CLEAR_BACKGROUND);

            BeginMode2D(m_Camera);
                // ** Render **
                auto view = m_Registry.view<Transform_C>();
                for (auto entity : view) {
                    auto &transform = view.get<Transform_C>(entity);
                    RenderTransform(transform);
                }
            EndMode2D();

            // Render GUI here

        EndDrawing();
    }

    void Scene::RenderTransform(Transform_C &transform) {
        #ifdef AC_RENDER_ENTITY_HITBOX
            DrawRectangleLines(int(transform.rec.x),
                                int(transform.rec.y),
                                int(transform.rec.width),
                                int(transform.rec.height),
                                transform.color);
        #endif

        #ifdef AC_RENDER_ENTITY_TAG
                DrawText(transform.tag.c_str(), 
                int(transform.rec.x), 
                int(transform.rec.y - 20),      // little offset so tag will display above rec
                AC_RENDER_ENTITY_TAG_FONT_SIZE, 
                AC_RENDER_ENTITY_TAG_FONT_COLOR);
        #endif
    }
}