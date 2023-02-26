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

#include "acaer/Scene/Scene.h"
#include "acaer/Scene/Entity.h"
#include "acaer/Scene/Components.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Entity Browser panel
     *        Displays all enities in current scene  
     */
    class EntityBrowserPanel {

	public:
        
		EntityBrowserPanel() = default;

        /**
         * @brief Construct a new Entity Browser Panel object
         * 
         * @param context current scene
         */
		EntityBrowserPanel(const Ref<Scene> &context);

        /**
         * @brief Set the Context object
         * 
         * @param context current scene
         */
		void SetContext(const Ref<Scene> &context);

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
		Ref<Scene> m_Context;           // Current scene
        Entity m_SelectionContext;      // Current selectet entity
    };
}