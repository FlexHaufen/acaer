/**
 * @file SceneEditorPanel.cpp
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/ImGui/Panels/SceneEditorPanel.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    void SceneEditorPanel::OnImGuiRender() {
		ImGui::Begin("Scene Editor Panel");

        if (m_Context) {

            ImGui::SeparatorText("Camera");

            static b8 c1 = false;
            ImGui::Checkbox("Free Camera", &c1);
            m_Context->m_useFreeCamera = c1;

        }
		ImGui::End();
	}
}