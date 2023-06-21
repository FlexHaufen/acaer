/**
 * @file ImGuiLayer.h
 * @author flexhaufen
 * @brief ImGui Layer
 * 		  NOTE: ImGui has to be on docking branch
 * @version 0.1
 * @date 2022-09-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

//*** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/ImGui/ImGuiPanelStack.h"

// * panels *
#include "acaer/ImGui/Panels/EntityBrowserPanel.h"
#include "acaer/ImGui/Panels/SceneEditorPanel.h"


//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {

	class ImGuiLayer { 
	public:
		ImGuiLayer(sf::RenderWindow& window);
		~ImGuiLayer() = default;

		void OnAttach(const Ref<Scene> &context);
		void OnDetach();

		void OnUpdate(sf::Time dt);
		void OnRender();

	private:
		void SetStyle();

	private:
		sf::RenderWindow &m_Window;
		ImGuiPanelStack m_PanelStack;

		// * panels *
		EntityBrowserPanel* m_pEntityBrowser = nullptr;
		SceneEditorPanel* m_pSceneEditor = nullptr;
	};
}