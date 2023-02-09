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
        Render();
    }

    void Scene::Render() {
        BeginDrawing();
            ClearBackground(AC_SCENE_CLEAR_BACKGROUND);

            // Update camera here
            m_Camera.target = {300, 100};

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
            DrawRectangleLines(int(transform.hitbox.x),
                                int(transform.hitbox.y),
                                int(transform.hitbox.width),
                                int(transform.hitbox.height),
                                transform.color);
        #endif

        #ifdef AC_RENDER_ENTITY_TAG
                DrawText(transform.tag.c_str(), 
                int(transform.hitbox.x), 
                int(transform.hitbox.y - 20),      // little offset so tag will display above hitbox
                AC_RENDER_ENTITY_TAG_FONT_SIZE, 
                AC_RENDER_ENTITY_TAG_FONT_COLOR);
        #endif
    }
}