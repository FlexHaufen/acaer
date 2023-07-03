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


// ** Scripts **
#include "acaer/Scripts/Player/CharacterController.h"


// *** DEFINE ***
//#define AC_SCENE_LOAD_ON_OPEN                           // flag
#define AC_SCENE_SAVE_ON_CLOSE                          // flag

// *** NAMESPACE ***
namespace Acaer {

    Core::Core() {
        Log::Init();

        // random number seed
        srand((u32)time(NULL));

        AC_CORE_INFO("Initializing Core");

        #ifdef AC_PROFILE
            AC_CORE_WARN("Profiler is enabled and may use unnecessary recources");
            AC_PROFILE_BEGIN_SESSION("Profile", "AcaerProfile.json");
        #endif

        std::filesystem::current_path("./");
        AC_CORE_INFO("Running in: {0}",std::filesystem::current_path());
        
        AC_CORE_INFO("Creating main window");
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
        
        m_ActiveScene = CreateRef<Scene>(m_Window);
        m_EventManager = CreateRef<EventManager>(m_Window);
        m_ImGuiLayer = CreateRef<ImGuiLayer>(m_Window);
        
        m_ImGuiLayer->OnAttach(m_ActiveScene);

    #ifdef AC_SCENE_LOAD_ON_OPEN
        AC_CORE_INFO("Loading scene...");
        // Load Scene
        SceneSerializer serializer(m_ActiveScene);
        if (!serializer.Deserialize("assets/Scenes/scene.acs")) {
            AC_CORE_ERROR("Failed to load scene");
        }
    #else
        //! ---- DEBUG ----
        {   // Player
            AC_CORE_TRACE("creating player");
            auto player = m_ActiveScene->CreateEntity("player");
            auto &t = player.AddComponent<Component::Transform>();
            t.pos = {150, 0};
            auto &cam = player.AddComponent<Component::CameraController>();
            cam.zoom = 1.2f;
            player.AddComponent<Component::NativeScript>().Bind<CharacterController>();
            auto &rb = player.AddComponent<Component::RigidBody>();
            rb.type = Component::RigidBody::BodyType::Dynamic;
            rb.density = 5.f;
            rb.friction = 0.9f;
            auto &c = player.AddComponent<Component::Collider>();
            c.size = {192, 192};

            Component::Collider::Sensor sns;
            sns.userData->name = "foot";
            sns.size = {60, 10};
            sns.offset = {0, 192};
            c.sensors.emplace(sns.userData->name, sns);

            auto &s = player.AddComponent<Component::Sprite>();
            s.texturepath = "assets/Textures/Player/run_anim_temp-Sheet.png";
            auto &sa = player.AddComponent<Component::SpriteAnimatior>();
            sa.currentAnimation = "run_left";
            sa.pool.emplace("run_right", Component::SpriteAnimatior::Animation{
                                            .framePos = 0,
                                            .frameLenght = 8,
                                            .frameSize {48, 48}
                                        });
            sa.pool.emplace("run_left", Component::SpriteAnimatior::Animation{
                                            .framePos = 0,
                                            .frameLenght = 8,
                                            .frameSize {48, 48},
                                            .isMirrored = true
                                        });
        }
        {   // Ent1
            AC_CORE_TRACE("creating gnd");
            auto ent = m_ActiveScene->CreateEntity("gnd");
            auto &t = ent.AddComponent<Component::Transform>();
                t.pos = {100, 150};
            auto &rb = ent.AddComponent<Component::RigidBody>();
                rb.type = Component::RigidBody::BodyType::Static;

            auto &c = ent.AddComponent<Component::Collider>();
                c.size = {400, 40};

            auto &s = ent.AddComponent<Component::Sprite>();
                s.texturepath = "assets/Textures/Debug/platform.png";
        }
        {   // tree
            AC_CORE_TRACE("tree1");
            auto ent = m_ActiveScene->CreateEntity("tree1");
            auto &t = ent.AddComponent<Component::Transform>();
                t.pos = {50, -444};

            auto &s = ent.AddComponent<Component::Sprite>();
                s.texturepath = "assets/Textures/World/fir_tree_1.png";
        }
        {   // tree
            AC_CORE_TRACE("tree2");
            auto ent = m_ActiveScene->CreateEntity("tree2");
            auto &t = ent.AddComponent<Component::Transform>();
                t.pos = {150, -444};

            auto &s = ent.AddComponent<Component::Sprite>();
                s.texturepath = "assets/Textures/World/fir_tree_1.png";
        }
        {   // tree
            AC_CORE_TRACE("tree3");
            auto ent = m_ActiveScene->CreateEntity("tree3");
            auto &t = ent.AddComponent<Component::Transform>();
                t.pos = {-300, -444};

            auto &s = ent.AddComponent<Component::Sprite>();
                s.texturepath = "assets/Textures/World/fir_tree_1.png";
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

        AC_PROFILE_END_SESSION();  
    }

    void Core::Run() {
        AC_PROFILE_FUNCTION();

        sf::Clock dt_clock;
        sf::Time dt;
        
        AC_CORE_INFO("Setting up EventManager");
        m_EventManager->addEventCallback(sf::Event::EventType::Closed, [&](const sf::Event&) { m_Window.close(); });
        m_EventManager->addKeyPressedCallback(sf::Keyboard::Key::Escape, [&](const sf::Event&) { m_Window.close(); });
        m_EventManager->addKeyPressedCallback(sf::Keyboard::Tab, [&](const sf::Event&) { m_isPaused = !m_isPaused; });
        
        AC_CORE_INFO("Starting scene...");
        m_ActiveScene->OnStart();
        while (m_Window.isOpen() && m_isRunning) {
            
            // ---- EVENT HANDLING ----
            m_EventManager->processEvents(nullptr);
            //ImGui::SFML::ProcessEvent();          // TODO: Add ImGui Events

            if (!m_isPaused) {
                dt = dt_clock.restart();
                f32 dt_sec = dt.asSeconds();
                u16 fps = u16(1/dt_sec);
                m_Window.setTitle(m_WindowTitle + " - FPS: " + std::to_string(fps));

                // ---- UPDATE HANDLING ----
                m_ActiveScene->OnUpdate(dt_sec);
                m_ImGuiLayer->OnUpdate(dt);

                // ---- RENDER LOOP ----
                m_Window.clear(AC_SCENE_CLEAR_BACKGROUND);
                m_ActiveScene->OnRender(dt_sec);
                
                m_ImGuiLayer->OnRender();
                m_Window.display();
            } 
            else {
                m_Window.setTitle(m_WindowTitle + " - PAUSED");
            }
        }
        m_ActiveScene->OnEnd();
        AC_CORE_WARN("Core stopped running");
    }
}