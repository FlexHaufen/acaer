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
# pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Scene.h"



//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    /**
     * @brief Core
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



    private:
      
        bool m_isRunning = true;    // True when app is running
        bool m_isMinimized = false; // True when app is minimzed

        Ref<Scene> m_ActiveScene;   // Current active scene

    };
}