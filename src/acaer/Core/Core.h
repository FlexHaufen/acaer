/**
 * @file Core.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-02-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Scene.h"


#include "acaer/ImGui/ImGuiLayer.h"
#include "acaer/ImGui/Panels/EntityBrowserPanel.h"

#include "acaer/Core/Events/EventManager.h"

//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Core class of Game
     * 
     */
    class Core {

    public:

        /**
         * @brief Construct a new Core object
         * 
         */
        Core();

        /**
         * @brief Destroy the Core object
         * 
         */
        ~Core();

        /**
         * @brief Running loop
         * 
         */
        void Run();

        /**
         * @brief Closes App
         * 
         */
        void Close();

        /**
         * @brief Get the EventManager
         * 
         * @return Ref<EventManager> EventManager
         */
        Ref<EventManager> GetEventManager() { return m_EventManager; }
    private:
      
        // ** Members **
        bool m_isRunning = true;        // True when app is running
        bool m_isPaused = false;        // True when app is paused
        bool m_isMinimized = false;     // True when app is minimzed

        sf::RenderWindow m_Window;      // Sfml window
        std::string m_WindowTitle;      // Sfml window title

        Ref<EventManager> m_EventManager;   // Event manager
        Ref<Scene> m_ActiveScene;           // Current active scene

        Ref<ImGuiLayer> m_ImGuiLayer;       // Imgui    (for debug)
    };
}