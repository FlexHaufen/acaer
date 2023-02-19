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
#include "acaer/Scene/ScriptableEntity.h"
#include "acaer/Scene/SceneSerializer.h"

#include "acaer/Core/Events/EventManager.h"

// ** Scripts **
#include "acaer/Scripts/Player/CharacterController.h"


// *** DEFINE ***

// *** NAMESPACE ***
namespace Acaer {

    Core::Core() {

        m_Window.create(sf::VideoMode(AC_WINDOW_X, AC_WINDOW_Y), "acaer");

        //m_ImGuiLayer->OnAttach();
        m_ActiveScene = CreateRef<Scene>();

    #ifdef AC_SCENE_LOAD_ON_OPEN
        // Load Scene
        SceneSerializer serializer(m_ActiveScene);
        if (!serializer.Deserialize("assets/Scenes/scene.acs")) {
            std::cout << "fuck - no scene";
        }
    #else
        //! ---- DEBUG ----
        auto ent1 = m_ActiveScene->CreateEntity("ent1");
        auto &t1 = ent1.AddComponent<Transform_C>();
        t1.pos = {100, 100};
        t1.size = {100, 200};
        t1.color = {255, 17, 0, 255};

        auto ent2 = m_ActiveScene->CreateEntity("ent2");
        auto &t2 = ent2.AddComponent<Transform_C>();
        t2.pos = {400, 150};
        t2.size = {200, 200};
        t2.color = {0, 251, 255, 255};

        auto player = m_ActiveScene->CreateEntity("player");
        auto &t3 = player.AddComponent<Transform_C>();


        t3.pos = {300, 100};
        t3.size = { 50, 100};
        t3.color = {34, 255, 0, 255};
        player.AddComponent<Input_C>();
        player.AddComponent<Camera_C>();
        player.AddComponent<NativeScript_C>().Bind<CharacterController>();
        
        //! ----------------
    #endif

        m_isRunning = true;
    }

    Core::~Core() {
        // Save scene
        #ifdef AC_SCENE_SAVE_ON_CLOSE
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize("assets/Scenes/scene.acs");
        #endif

        //m_ImGuiLayer->OnDetach();

        m_Window.close();
    }

    void Core::Run() {
        sf::Clock dt_clock;
        
        EventManager eventManager(m_Window);
        eventManager.addEventCallback(sf::Event::EventType::Closed, [&](const sf::Event&) {m_Window.close(); });
        eventManager.addKeyPressedCallback(sf::Keyboard::Key::Escape, [&](const sf::Event&) {m_Window.close(); });


        while (m_Window.isOpen() && m_isRunning) {
            
            f32 dt = dt_clock.restart().asSeconds();
           
            #ifdef AC_CALC_FPS
                u16 fps = u16(1/dt);
                m_Window.setTitle("arcaer - FPS: " + std::to_string(fps));
            #endif

            // ---- EVENT HANDLING ----
            eventManager.processEvents();
           
            // TODO: Add input for player;
            //m_ActiveScene->OnInput(dt, );


            m_ActiveScene->OnUpdate(dt, m_Window);

            // ---- RENDER LOOP ----
            m_Window.clear(AC_SCENE_CLEAR_BACKGROUND);
            m_ActiveScene->OnRender(m_Window);
            //    m_ImGuiLayer->Begin();
                //ImGui::ShowDemoWindow();
            //    m_ImGuiLayer->End();
            m_Window.display();
        }
    }
}