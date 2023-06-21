/**
 * @file ImGuiPanelStack.h
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
#include "acaer/ImGui/ImGuiPanel.h"


//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {

	class ImGuiPanelStack {
	public:
		//ImGuiPanelStack() = default;
		//~ImGuiPanelStack();

		void PushPanel(ImGuiPanel* panel) {
            m_Panels.emplace(m_Panels.begin() + m_PanelInsertIndex, panel);
            m_PanelInsertIndex++;
        }

		void PopPanel(ImGuiPanel* panel) {
            auto it = std::find(m_Panels.begin(), m_Panels.begin() + m_PanelInsertIndex, panel);
            if (it != m_Panels.begin() + m_PanelInsertIndex) {
                m_Panels.erase(it);
                m_PanelInsertIndex--;
            }
        }

		std::vector<ImGuiPanel*>::iterator begin() { return m_Panels.begin(); }
		std::vector<ImGuiPanel*>::iterator end() { return m_Panels.end(); }
		std::vector<ImGuiPanel*>::reverse_iterator rbegin() { return m_Panels.rbegin(); }
		std::vector<ImGuiPanel*>::reverse_iterator rend() { return m_Panels.rend(); }

		std::vector<ImGuiPanel*>::const_iterator begin() const { return m_Panels.begin(); }
		std::vector<ImGuiPanel*>::const_iterator end()	const { return m_Panels.end(); }
		std::vector<ImGuiPanel*>::const_reverse_iterator rbegin() const { return m_Panels.rbegin(); }
		std::vector<ImGuiPanel*>::const_reverse_iterator rend() const { return m_Panels.rend(); }
	private:
		std::vector<ImGuiPanel*> m_Panels;			// Panel Vector
		u32 m_PanelInsertIndex = 0;
	};
}