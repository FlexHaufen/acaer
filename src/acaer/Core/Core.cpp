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

#include "acaer/Scene/Entity/Components.h"
#include "acaer/Scene/Entity/Entity.h"
#include "acaer/Scene/Entity/ScriptableEntity.h"
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
        m_WindowTitle = "acaer v";
        m_WindowTitle.append(AC_VERSION);
        m_Window.create(sf::VideoMode(AC_WINDOW_X, AC_WINDOW_Y), m_WindowTitle);

        sf::Image icon;
        if (!icon.loadFromFile(AC_WINDOW_ICON_PATH)) {
            AC_CORE_WARN("Can't open application icon");
        }
        else {
            m_Window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        }
        
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
            auto &t = ent.AddComponent<Component::Transform>();
            t.pos = {100, 150};
            auto &rb = ent.AddComponent<Component::RigidBody>();
            rb.type = Component::RigidBody::BodyType::Static;

            auto &c = ent.AddComponent<Component::Collider>();
            c.size = {400, 40};

            auto &s = ent.AddComponent<Component::Sprite>();
            if (!s.texture.loadFromFile("assets/Textures/Debug/platform.png")) {
                AC_CORE_WARN("Couldn't load sprite texture");
            }
        }
        {   // tree
            AC_CORE_TRACE("tree");
            auto ent = m_ActiveScene->CreateEntity("tree");
            auto &t = ent.AddComponent<Component::Transform>();
            t.pos = {150, -444};

            auto &s = ent.AddComponent<Component::Sprite>();
            if (!s.texture.loadFromFile("assets/Textures/World/fir_tree_1.png")) {
                AC_CORE_WARN("Couldn't load sprite texture");
            }
        }
        {   // Player
            AC_CORE_TRACE("creating player");
            auto player = m_ActiveScene->CreateEntity("player");
            auto &t = player.AddComponent<Component::Transform>();
            t.pos = {150, 0};
            player.AddComponent<Component::Input>();
            auto &cam = player.AddComponent<Component::Camera>();
            cam.zoom = 1.2f;
            player.AddComponent<Component::NativeScript>().Bind<CharacterController>();
            auto &rb = player.AddComponent<Component::RigidBody>();
            rb.type = Component::RigidBody::BodyType::Dynamic;
            rb.density = 5.f;
            rb.friction = 0.9f;
            auto &c = player.AddComponent<Component::Collider>();
            c.size = {44, 150};
            //c.offset = {10, 10}
            auto &s = player.AddComponent<Component::Sprite>();
            if (!s.texture.loadFromFile("assets/Textures/Player/player_raw.png")) {
                AC_CORE_WARN("Couldn't load sprite texture");
            }
        }
        //! ----------------
    #endif
        m_EntityBrowserPanel.SetContext(m_ActiveScene);
        m_isRunning = true;

        // random number seed
        srand((u32)time(NULL));
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
        
        AC_CORE_INFO("Starting scene...");
        m_ActiveScene->OnStart();
        while (m_Window.isOpen() && m_isRunning) {
            
            dt = dt_clock.restart();
            f32 dt_sec = dt.asSeconds();
           
            #ifdef AC_CALC_FPS
                u16 fps = u16(1/dt_sec);
                m_Window.setTitle(m_WindowTitle + " - FPS: " + std::to_string(fps));
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
            
            //ImGui::ShowDemoWindow();
            m_EntityBrowserPanel.OnImGuiRender();

            m_ImGuiLayer->OnRender(m_Window);
            m_Window.display();
        }
        m_ActiveScene->OnEnd();
        AC_CORE_WARN("Core stopped runnging");
    }
}