/**
 * @file EntityBrowserPanel.cpp
 * @author flexhaufen
 * @brief Entity Browser panel
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/ImGui/Panels/EntityBrowserPanel.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    void EntityBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Entity Browser Panel");

		if (m_Context) {
            ImGui::Text("Entities:");
            static UUID currentEntity;
            if (ImGui::BeginListBox("##entity_listBox", ImVec2(-FLT_MIN, 0))) {

                m_Context->m_Registry.view<Component::Tag>().each([&](auto entityID, auto& tag) {
                    Entity entity(entityID, m_Context.get());

                    const b8 is_selected = (currentEntity == entity.GetUUID());
                    if (ImGui::Selectable(entity.GetTag().c_str(), is_selected)) {
                        currentEntity = entity.GetUUID();
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                        m_SelectionContext = entity;
                        //AC_CORE_TRACE("Selected: {0}", entity.GetTag());
                    }   
                });
                ImGui::EndListBox();
            }
			
            if (ImGui::Button("Add Entity")) {
                m_Context->CreateEntity();
            }
            ImGui::SameLine();
            if (ImGui::Button("Remove Entity")) {
                m_Context->DestroyEntity(m_SelectionContext);
                m_SelectionContext = {};
            }
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);

            // TODO (flex): Add "add component" button here
		}
		ImGui::End();
	}


    void EntityBrowserPanel::DrawEntityNode(Entity &entity) {
        auto &tag = entity.GetComponent<Component::Tag>().tag;


    }

    void EntityBrowserPanel::DrawComponents(Entity &entity) {

        if (entity.HasComponent<Component::Tag>()) {
            ImGui::SeparatorText("Tag");
            auto &tag = entity.GetComponent<Component::Tag>().tag;
            auto &uuid = entity.GetComponent<Component::Tag>().uuid;         

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));  // very dangerous
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
            if (ImGui::InputText("Tag: ", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }

            ImGui::Text("UUID: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), std::to_string(uuid).c_str());

            ImGui::Text("entt::entity: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), std::to_string((u32)entity).c_str());
        }

        if (entity.HasComponent<Component::Transform>()) {
            ImGui::SeparatorText("Transform");
            auto &transform = entity.GetComponent<Component::Transform>();

            ImGui::SliderInt("render layer", (s32*)&transform.renderLayer, 0, 9);

            f32 f1[2] = {transform.pos.x, transform.pos.y};
            ImGui::InputFloat2("pos", f1);
            transform.pos = {f1[0], f1[1]};

            f32 f2[2] = {transform.scale.x, transform.scale.y};
            ImGui::InputFloat2("scale", f2);
            transform.scale = {f2[0], f2[1]};

            ImGui::InputFloat("Rotation", &transform.rotation);
        }

        if (entity.HasComponent<Component::RigidBody>()) {
            ImGui::SeparatorText("Rigid Body");
            auto &rigidBody = entity.GetComponent<Component::RigidBody>();

            ImGui::Checkbox("Fixed Rotation", &rigidBody.fixedRoation);
            ImGui::InputFloat("Denisty", &rigidBody.density);
            ImGui::InputFloat("Friction", &rigidBody.friction);
            ImGui::InputFloat("Restitution", &rigidBody.restitution);
            ImGui::InputFloat("Restitution TH", &rigidBody.restitutionThreshold);
        }

        if (entity.HasComponent<Component::Sprite>()) {
            ImGui::SeparatorText("Sprite");
            auto &sprite = entity.GetComponent<Component::Sprite>();

            ImGui::Text("Texture Path: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), sprite.texturepath.c_str());

            ImGui::Text("Texture Loaded: ");
            ImGui::SameLine();
            b8 b = (sprite.spriteTexture.getTexture() != nullptr);
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), Convert::b8_to_abc(b).c_str());
        }

        if (entity.HasComponent<Component::SpriteAnimatior>()) {
            ImGui::SeparatorText("Sprite");
            auto &spriteAnim = entity.GetComponent<Component::SpriteAnimatior>();

            ImGui::Text("Current Animation: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), spriteAnim.currentAnimation.c_str());
        
            ImGui::Text("Animation Pool Size: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), std::to_string(spriteAnim.pool.size()).c_str());
        }

        if (entity.HasComponent<Component::CameraController>()) {
            ImGui::SeparatorText("Camera Controller");
            auto &camera = entity.GetComponent<Component::CameraController>();
            ImGui::SliderFloat("zoom", &camera.zoom, 0.5, 2);
        }
    }
}