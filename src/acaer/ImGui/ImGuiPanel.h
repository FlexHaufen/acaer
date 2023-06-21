/**
 * @file ImGuiPanel.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

//*** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Scene.h"

//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {

	class ImGuiPanel {
	public:
		ImGuiPanel(const std::string& name = "Panel") : m_DebugName(name) {};
		virtual ~ImGuiPanel() = default;

		/**
         * @brief Set Context
         * 
         * @param context current scene
         */
		virtual void SetContext(const Ref<Scene> &context) { m_Context = context; }

        /**
         * @brief ImGui Render
         * 
         */
		virtual void OnImGuiRender() = 0;

		const std::string& GetName() const { return m_DebugName; }
	protected:
		// ** Members **  
		std::string m_DebugName;
		Ref<Scene> m_Context;           // Current scene
	};
}