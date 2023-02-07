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
#include "acaer/Scene/Components.h"
#include "acaer/Scene/Entity.h"



// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    Scene::Scene() {

        entt::entity entity = m_Registry.create();
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
        auto view = m_Registry.view<Transform_C>();
        BeginDrawing();
            ClearBackground(AC_SCENE_CLEAR_BACKGROUND);
        
        for (auto entity : view) {
            auto &transform = view.get<Transform_C>(entity);
    
            #ifdef AC_RENDER_ENTITY_TAG
                DrawText(transform.tag.c_str(), 
                         int(transform.hitbox.x), 
                         int(transform.hitbox.y - 20),      // little offset so tag will display above hitbox
                         AC_RENDER_ENTITY_TAG_FONT_SIZE, 
                         AC_RENDER_ENTITY_TAG_FONT_COLOR);
            #endif

            #ifdef AC_RENDER_ENTITY_HITBOX
                DrawRectangleLines(int(transform.hitbox.x),
                                int(transform.hitbox.y),
                                int(transform.hitbox.width),
                                int(transform.hitbox.height),
                                transform.color);
            #endif

        }
        EndDrawing();
    }
}