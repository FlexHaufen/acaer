/**
 * @file EventManager.h
 * @author flexhaufen
 * @brief Event Manager for SFML
 * @version 0.1
 * @date 2023-02-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"


// *** NAMESPACE
namespace Acaer {

    // Helper using for shorter types
    using EventCallback = std::function<void(const sf::Event& event)>;

    template<typename T>
    using EventCallbackMap = std::unordered_map<T, EventCallback>;


    /**
     * @brief This class handles subtyped events like keyboard or mouse events
     *        The unpack function allows to get relevant information from the processed event
     * 
     * @tparam T 
     */
    template<typename T>
    class SubTypeManager {
    public:

        SubTypeManager(std::function<T(const sf::Event&)> unpack);

        ~SubTypeManager() = default;

        void processEvent(const sf::Event& event) const;

        void addCallback(const T& sub_value, EventCallback callback);

    private:

        // ** Members **
        EventCallbackMap<T> m_callmap;
        std::function<T(const sf::Event&)> m_unpack;
    };

    class EventMap {
    public:
        EventMap(bool use_builtin_helpers = true);
        
        /**
         * @brief Attaches new callback to an event
         * 
         * @param type      Event type
         * @param callback  Callback
         */
        void addEventCallback(sf::Event::EventType type, EventCallback callback);
        
        /**
         * @brief Adds a key pressed callback
         * 
         * @param key_code  Keycode
         * @param callback  Callback
         */
        void addKeyPressedCallback(sf::Keyboard::Key key_code, EventCallback callback);

        /**
         * @brief Adds a key releasd callback
         * 
         * @param key_code Keycode
         * @param callback Callback
         */
        void addKeyReleasedCallback(sf::Keyboard::Key key_code, EventCallback callback);

        /**
         * @brief Adds a mouse pressed callback
         * 
         * @param button    Mouse button
         * @param callback  Callback
         */
        void addMousePressedCallback(sf::Mouse::Button button, EventCallback callback);

        /**
         * @brief Adds a mouse released callback
         * 
         * @param button    Mouse button
         * @param callback  Callback
         */
        void addMouseReleasedCallback(sf::Mouse::Button button, EventCallback callback);
        
        /**
         * @brief Runs the callback associated with an event
         * 
         * @param e         SFML event
         * @param callback  Callback
         */
        void executeCallback(const sf::Event& e, EventCallback callback = nullptr) const;
        
        /**
         * @brief Removes a callback
         * 
         * @param type  Event type
         */
        void removeCallback(sf::Event::EventType type);
        
    private:

        // ** Members **
        SubTypeManager<sf::Keyboard::Key> m_key_pressed_manager;
        SubTypeManager<sf::Keyboard::Key> m_key_released_manager;
        SubTypeManager<sf::Mouse::Button> m_mouse_pressed_manager;
        SubTypeManager<sf::Mouse::Button> m_mouse_released_manager;
        EventCallbackMap<sf::Event::EventType> m_events_callmap;
    };


    /**
     * @brief This class handles any type of event and call its associated callbacks if any.
     *        To process key event in a more convenient way its using a KeyManager
     * 
     */
    class EventManager {
    public:
        EventManager(sf::Window &window, bool use_builtin_helpers = true) :
            m_window(window),
            m_event_map(use_builtin_helpers) {
        }

        /**
         * @brief Calls events' attached callbacks
         * 
         * @param callback 
         */
        void processEvents(EventCallback callback = nullptr) const {
            // Iterate over events
            sf::Event event;
            while (m_window.pollEvent(event)) {
                m_event_map.executeCallback(event, callback);
            }
        }
        
        /**
         * @brief Attaches new callback to an event
         * 
         * @param type 
         * @param callback 
         */
        void addEventCallback(sf::Event::EventType type, EventCallback callback) {
            m_event_map.addEventCallback(type, callback);
        }

        /**
         * @brief Removes a callback
         * 
         * @param type 
         */
        void removeCallback(sf::Event::EventType type) {
            m_event_map.removeCallback(type);
        }

        /**
         * @brief Adds a key pressed callback
         * 
         * @param key 
         * @param callback 
         */
        void addKeyPressedCallback(sf::Keyboard::Key key, EventCallback callback) {
            m_event_map.addKeyPressedCallback(key, callback);
        }

        /**
         * @brief Adds a key released callback
         * 
         * @param key 
         * @param callback 
         */
        void addKeyReleasedCallback(sf::Keyboard::Key key, EventCallback callback) {
            m_event_map.addKeyReleasedCallback(key, callback);
        }

        /**
         * @brief Adds a mouse pressed callback
         * 
         * @param button 
         * @param callback 
         */
        void addMousePressedCallback(sf::Mouse::Button button, EventCallback callback) {
            m_event_map.addMousePressedCallback(button, callback);
        }

        /**
         * @brief Adds a mouse released callback
         * 
         * @param button 
         * @param callback 
         */
        void addMouseReleasedCallback(sf::Mouse::Button button, EventCallback callback) {
            m_event_map.addMouseReleasedCallback(button, callback);
        }
        
        sf::Window& getWindow() {return m_window;}

        sf::Vector2f getFloatMousePosition() const {
            const sf::Vector2i mouse_position = sf::Mouse::getPosition(m_window);
            return { static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y) };
        }

        sf::Vector2i getMousePosition() const {
            return sf::Mouse::getPosition(m_window);
        }

    private:
        // ** Members **
        sf::Window& m_window;
        EventMap    m_event_map;
    };

}