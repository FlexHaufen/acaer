/**
 * @file Core.cpp
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-02-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Core/Core.h"
#include "acaer/Scene/Components.h"
#include "acaer/Scene/Entity.h"



// *** DEFINE ***
#define AC_WINDOW_RESIZABLE

// *** NAMESPACE ***
namespace Acaer {

    Core::Core() {
        SetTraceLogLevel(0);
        #ifdef AC_WINDOW_RESIZABLE
            SetConfigFlags(FLAG_WINDOW_RESIZABLE);    // Window configuration flags
        #endif

        InitWindow(800, 450, "acaer");

        if (!IsWindowReady()) {
            TraceLog(LOG_FATAL, "Couldn't create Window");
        }

        m_ActiveScene = CreateRef<Scene>();
        auto ent1 = m_ActiveScene->CreateEntity();
        auto &t1 = ent1.GetComponent<Transform_C>();
        t1.hitbox = {100, 100, 100, 200};
        t1.color = RED;
        auto ent2 = m_ActiveScene->CreateEntity();
        auto &t2 = ent2.GetComponent<Transform_C>();
        t2.hitbox = {80, 200, 300, 300};
        t2.color = PINK;
    }

    Core::~Core() {
        CloseWindow();
    }

    void Core::Run() {
        std::string windowTitel;
        
        m_isRunning = true;
        while (!WindowShouldClose()) {
            
            f32 dt = GetFrameTime();
           
            windowTitel = "arcaer - FPS: " + std::to_string(GetFPS());
            SetWindowTitle(windowTitel.c_str());
        
            if (IsWindowMinimized()) {
                
                
            }


            m_ActiveScene->OnUpdate(dt);

        }
        m_isRunning = false;
    }
}