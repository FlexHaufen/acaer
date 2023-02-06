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



// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    Core::Core() {

        InitWindow(800, 450, "acaer");
        //SetTargetFPS(144);
       
    }

    Core::~Core() {
        CloseWindow();
    }

    void Core::Run() {
        std::string windowTitel;
        
        m_isRunning = true;
        while (!WindowShouldClose()) {
            
           
            windowTitel = "arcaer - FPS: " + std::to_string(GetFPS());
            SetWindowTitle(windowTitel.c_str());
        
			


          

            

            if (IsWindowMinimized()) {
                
                
            }


            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            EndDrawing();
        }
        m_isRunning = false;
    }
}