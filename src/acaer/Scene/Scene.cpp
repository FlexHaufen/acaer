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


    Entity Scene::CreateEntity() {
        Entity entity(m_Registry.create(), this);

        entity.AddComponent<Transform_C>();

        return entity;
    }



    void Scene::OnUpdate(f32 dt) {
        auto view = m_Registry.view<Transform_C>();
        BeginDrawing();
            ClearBackground(WHITE);
        
        for (auto entity : view) {
            auto &transform = view.get<Transform_C>(entity);
    
            DrawRectangleLines(int(transform.hitbox.x),
                               int(transform.hitbox.y),
                               int(transform.hitbox.width),
                               int(transform.hitbox.height),
                               transform.color);


        }
        EndDrawing();
    }
}