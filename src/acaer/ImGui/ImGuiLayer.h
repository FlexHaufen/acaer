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


//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {

	class ImGuiLayer { 
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		void OnAttach();
		void OnDetach();

		void Begin();
		void End();

		void SetStyle();
	private:
	};
}
