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

    private:
    
        /**
         * @brief Game loop of app
         * 
         */
        void OnGameUpdate();

        /**
         * @brief Performe a HotReload
         * 
         */
        void PerformHotReload();

    private:
        // ** Members **
        sf::Clock m_dt_clock;           // Sfml Clock for DeltaTime
        sf::Time m_dt;                  // DeltaTime

        b8 m_isRunning = true;          // True when app is running
        b8 m_isPaused = false;          // True when app is paused
        b8 m_isMinimized = false;       // True when app is minimzed
        b8 m_UpdateGame = true;         // True when game should be updated

        sf::RenderWindow m_Window;      // Sfml window
        std::string m_WindowTitle;      // Sfml window title

        EventManager m_EventManager;    // Event manager
        Ref<Scene> m_ActiveScene;       // Current active scene

        Ref<ImGuiLayer> m_ImGuiLayer;   // Imgui    (for debug)
    };
}