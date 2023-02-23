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
//#define AC_SCENE_LOAD_ON_OPEN                           // flag
#define AC_SCENE_SAVE_ON_CLOSE                          // flag

// *** NAMESPACE ***
namespace Acaer {

    Core::Core() {
        Log::Init();

        AC_CORE_INFO("ACAER v{0}", AC_VERSION);
        AC_CORE_INFO("----------------------");

        AC_CORE_INFO("Initializing Core");
        

        AC_CORE_INFO("Creating Window");
        m_Window.create(sf::VideoMode(AC_WINDOW_X, AC_WINDOW_Y), "acaer");

        m_ImGuiLayer->OnAttach(m_Window);
        m_ActiveScene = CreateRef<Scene>();

    #ifdef AC_SCENE_LOAD_ON_OPEN
        AC_CORE_INFO("Loading scene...");
        // Load Scene
        SceneSerializer serializer(m_ActiveScene);
        if (!serializer.Deserialize("assets/Scenes/scene.acs")) {
            AC_CORE_ERROR("Failed to load scene");
        }
    #else
        //! ---- DEBUG ----
        {   // Ent1
            AC_CORE_TRACE("creating ent1");
            auto ent = m_ActiveScene->CreateEntity("ent1");
            auto &t = ent.AddComponent<Transform_C>();
            t.pos = {100, 150};
            auto &rb = ent.AddComponent<RigidBody_C>();
            rb.type = RigidBody_C::BodyType::Static;

            auto &c = ent.AddComponent<Collider_C>();
            c.size = {100, 10};

            auto &s = ent.AddComponent<Sprite_C>();
            if (!s.texture.loadFromFile("assets/Textures/Debug/platform.png")) {
                AC_CORE_WARN("Couldn't load sprite texture");
            }
        }
        //{   // Ent2
        //    AC_CORE_TRACE("creating ent2");
        //    auto ent = m_ActiveScene->CreateEntity("ent2");
        //    auto &t = ent.AddComponent<Transform_C>();
        //    t.pos = {100, 50};
        //    //t.size = {10, 100};
        //    t.color = {0, 251, 255, 255};
        //    auto &rb = ent.AddComponent<RigidBody_C>();
        //    rb.type = RigidBody_C::BodyType::Static;
        //}
        {   // Player
            AC_CORE_TRACE("creating player");
            auto player = m_ActiveScene->CreateEntity("player");
            auto &t = player.AddComponent<Transform_C>();
            t.pos = {150, 0};
            player.AddComponent<Input_C>();
            player.AddComponent<Camera_C>();
            player.AddComponent<NativeScript_C>().Bind<CharacterController>();
            auto &rb = player.AddComponent<RigidBody_C>();
            rb.type = RigidBody_C::BodyType::Dynamic;
            rb.density = 200;
            auto &c = player.AddComponent<Collider_C>();
            c.size = {50, 100};
            //c.offset = {10, 10}


            auto &s = player.AddComponent<Sprite_C>();
            if (!s.texture.loadFromFile("assets/Textures/Player/player_raw.png")) {
                AC_CORE_WARN("Couldn't load sprite texture");
            }

        }
        //! ----------------
    #endif
        m_isRunning = true;
    }

    Core::~Core() {
        AC_CORE_INFO("Shutting down Core");
        // Save scene
        #ifdef AC_SCENE_SAVE_ON_CLOSE
            AC_CORE_INFO("Saving scene...");
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize("assets/Scenes/scene.acs");
        #endif

        m_Window.close();
        m_ImGuiLayer->OnDetach();        
    }

    void Core::Run() {
        sf::Clock dt_clock;
        sf::Time dt;
        
        AC_CORE_INFO("Setting up EventManager");
        EventManager eventManager(m_Window);
        eventManager.addEventCallback(sf::Event::EventType::Closed, [&](const sf::Event&) {m_Window.close(); });
        eventManager.addKeyPressedCallback(sf::Keyboard::Key::Escape, [&](const sf::Event&) {m_Window.close(); });

        m_ActiveScene->OnStart();
        while (m_Window.isOpen() && m_isRunning) {
            
            dt = dt_clock.restart();
            f32 dt_sec = dt.asSeconds();
           
            #ifdef AC_CALC_FPS
                u16 fps = u16(1/dt_sec);
                m_Window.setTitle("arcaer - FPS: " + std::to_string(fps));
            #endif

            // ---- EVENT HANDLING ----
            eventManager.processEvents(nullptr);
            //ImGui::SFML::ProcessEvent();          // TODO: Add ImGui Events

            // ---- UPDATE HANDLING ----
            m_ActiveScene->OnUpdate(dt_sec, m_Window);
            m_ImGuiLayer->OnUpdate(m_Window, dt);

            // ---- RENDER LOOP ----
            m_Window.clear(AC_SCENE_CLEAR_BACKGROUND);
            m_ActiveScene->OnRender(m_Window);
            
            b8 p = true;
            ImGui::ShowDemoWindow(&p);

            m_ImGuiLayer->OnRender(m_Window);
            m_Window.display();
        }
        m_ActiveScene->OnEnd();
        AC_CORE_WARN("Core stopped runnging");
    }
}