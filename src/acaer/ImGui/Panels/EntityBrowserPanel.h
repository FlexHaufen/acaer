/**
 * @file EntityBrowserPanel.h
 * @author flexhaufen
 * @brief Entity Browser panel
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"

#include "acaer/ImGui/ImGuiPanel.h"

#include "acaer/Scene/Entity/Entity.h"
#include "acaer/Scene/Entity/Components.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Entity Browser panel
     *        Displays all enities in current scene  
     */
    class EntityBrowserPanel : public ImGuiPanel {

	public:

        /**
         * @brief ImGui Render
         * 
         */
		void OnImGuiRender();

    private:
        /**
         * @brief Draws entity tree node
         * 
         * @param entity 
         */
        void DrawEntityNode(Entity entity);

        /**
         * @brief Draws entity components
         * 
         * @param entity 
         */
        void DrawComponents(Entity entity);

	private:
        // ** Members **  
        Entity m_SelectionContext;      // Current selectet entity
    };
}