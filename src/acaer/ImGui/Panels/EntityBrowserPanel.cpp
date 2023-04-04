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
    

    EntityBrowserPanel::EntityBrowserPanel(const Ref<Scene> &context) {
        m_Context = context;
    }

    void EntityBrowserPanel::SetContext(const Ref<Scene> &context) {
        m_Context = context;
    }

    void EntityBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Entity Browser Panel");

		if (m_Context) {
			m_Context->m_Registry.each([&](auto entityID) {
                Entity entity{ entityID , m_Context.get() };
                DrawEntityNode(entity);
            });

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
				m_SelectionContext = {};
            }
			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1)) {
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Context->CreateEntity("Empty Entity");
				ImGui::EndPopup();
			}
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}


    void EntityBrowserPanel::DrawEntityNode(Entity entity) {
        auto &tag = entity.GetComponent<Tag_C>().tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (m_SelectionContext == entity) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        b8 opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

        if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
		}

        bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted) {
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
    }

    void EntityBrowserPanel::DrawComponents(Entity entity) {

        if (entity.HasComponent<Tag_C>()) {
            ImGui::SeparatorText("Tag");
            auto &tag = entity.GetComponent<Tag_C>().tag;
            auto &uuid = entity.GetComponent<Tag_C>().uuid;         

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));  // very dangerous
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
            if (ImGui::InputText("Tag: ", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }

            ImGui::Text("UUID: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), std::to_string(uuid).c_str());
        }

        if (entity.HasComponent<Transform_C>()) {
            ImGui::SeparatorText("Transform");
            auto &transform = entity.GetComponent<Transform_C>();

            s32 i1 = transform.render_layer;
            ImGui::SliderInt("render layer", &i1, 0, 9);
            transform.render_layer = i1;

            f32 f1[2] = {transform.pos.x, transform.pos.y};
            ImGui::InputFloat2("pos", f1);
            transform.pos = {f1[0], f1[1]};

            f32 f2[2] = {transform.scale.x, transform.scale.y};
            ImGui::InputFloat2("scale", f2);
            transform.scale = {f2[0], f2[1]};

            f32 f3 = transform.rotation;
            ImGui::InputFloat("Rotation", &f3);
            f3 = transform.rotation;
        }

        if (entity.HasComponent<Camera_C>()) {
            ImGui::SeparatorText("Transform");
            auto &camera = entity.GetComponent<Camera_C>();

            f32 i1 = camera.zoom;
            ImGui::SliderFloat("zoom", &i1, 0.5, 2);
            camera.zoom = i1;
        }
    }
}