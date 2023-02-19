/**
 * @file EventManager.cpp
 * @author flexhaufen
 * @brief Event Manager for SFML
 * @version 0.1
 * @date 2023-02-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/Core/Events/EventManager.h"

// *** NAMESPACE
namespace Acaer {


    template<typename T>
    SubTypeManager<T>::SubTypeManager(std::function<T(const sf::Event&)> unpack) {
        m_unpack = unpack;
    }

    template<typename T>
    void SubTypeManager<T>::processEvent(const sf::Event& event) const {
        T sub_value = m_unpack(event);
        auto it(m_callmap.find(sub_value));
        if (it != m_callmap.end()) {
            // Call its associated callback
            (it->second)(event);
        }
    }

    template<typename T>
    void SubTypeManager<T>::addCallback(const T& sub_value, EventCallback callback) {
        m_callmap[sub_value] = callback;
    }


    EventMap::EventMap(bool use_builtin_helpers)
        : m_key_pressed_manager([](const sf::Event& event) {return event.key.code; })
        , m_key_released_manager([](const sf::Event& event) {return event.key.code; })
        , m_mouse_pressed_manager([](const sf::Event& event) {return event.mouseButton.button; })
        , m_mouse_released_manager([](const sf::Event& event) {return event.mouseButton.button; }) {
        if (use_builtin_helpers) {
            // Register key events built in callbacks
            this->addEventCallback(sf::Event::EventType::KeyPressed, [&](const sf::Event& event) {m_key_pressed_manager.processEvent(event); });
            this->addEventCallback(sf::Event::EventType::KeyReleased, [&](const sf::Event& event) {m_key_released_manager.processEvent(event); });
            this->addEventCallback(sf::Event::EventType::MouseButtonPressed, [&](const sf::Event& event) {m_mouse_pressed_manager.processEvent(event); });
            this->addEventCallback(sf::Event::EventType::MouseButtonReleased, [&](const sf::Event& event) {m_mouse_released_manager.processEvent(event); });
        }
    }


    void EventMap::addEventCallback(sf::Event::EventType type, EventCallback callback) {
        m_events_callmap[type] = callback;
    }
    
    void EventMap::addKeyPressedCallback(sf::Keyboard::Key key_code, EventCallback callback) {
        m_key_pressed_manager.addCallback(key_code, callback);
    }

    void EventMap::addKeyReleasedCallback(sf::Keyboard::Key key_code, EventCallback callback) {
        m_key_released_manager.addCallback(key_code, callback);
    }

    void EventMap::addMousePressedCallback(sf::Mouse::Button button, EventCallback callback) {
        m_mouse_pressed_manager.addCallback(button, callback);
    }

    void EventMap::addMouseReleasedCallback(sf::Mouse::Button button, EventCallback callback) {
        m_mouse_released_manager.addCallback(button, callback);
    }
    
    void EventMap::executeCallback(const sf::Event& e, EventCallback callback) const {
        auto it(m_events_callmap.find(e.type));
        if (it != m_events_callmap.end()) {
            // Call its associated callback
            (it->second)(e);
        } else if (callback) {
            callback(e);
        }
    }
    
    void EventMap::removeCallback(sf::Event::EventType type) {
        // If event type is registred
        auto it(m_events_callmap.find(type));
        if (it != m_events_callmap.end()) {
            // Remove its associated callback
            m_events_callmap.erase(it);
        }
    }
}