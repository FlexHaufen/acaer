/**
 * @file SceneEditorPanel.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"

#include "acaer/ImGui/ImGuiPanel.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Entity Browser panel
     *        Displays all enities in current scene  
     */
    class SceneEditorPanel : public ImGuiPanel {

	public:

        /**
         * @brief ImGui Render
         * 
         */
		void OnImGuiRender();
    };
}