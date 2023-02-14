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
#include "acaer/Scene/SceneSerializer.h"

// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    Core::Core() {
        SetTraceLogLevel(0);
        #ifdef AC_WINDOW_RESIZABLE
            SetConfigFlags(FLAG_WINDOW_RESIZABLE);    // Window configuration flags
        #endif

        InitWindow(AC_WINDOW_X, AC_WINDOW_Y, "acaer");

        if (!IsWindowReady()) {
            TraceLog(LOG_FATAL, "Couldn't create Window");
        }
    
        m_ActiveScene = CreateRef<Scene>();

        // Load Scene
        SceneSerializer serializer(m_ActiveScene);
        if (!serializer.Deserialize("assets/Scenes/scene.acs")) {
            std::cout << "fuck - no scene";
        }
        
    #if 0
        //! ---- DEBUG ----
        auto ent1 = m_ActiveScene->CreateEntity("ent1");
        auto &t1 = ent1.AddComponent<Transform_C>();
        t1.rec = {100, 100, 100, 200};
        t1.color = RED;

        auto ent2 = m_ActiveScene->CreateEntity("ent2");
        auto &t2 = ent2.AddComponent<Transform_C>();
        t2.rec = {400, 150, 200, 200};
        t2.color = BLUE;

        auto player = m_ActiveScene->CreateEntity("player");
        auto &t3 = player.AddComponent<Transform_C>();
        t3.rec = {300, 100, 50, 100};
        t3.color = PINK;
        player.AddComponent<Input_C>();
        player.AddComponent<Camera_C>();
        //! ----------------
    #endif
    }

    Core::~Core() {
        // Save scene
        SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize("assets/Scenes/scene.acs");
        
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