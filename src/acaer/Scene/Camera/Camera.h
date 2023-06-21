/**
 * @file Camera.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"

//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    class Camera {
   
    public:
        Camera() {
            m_View.setSize(sf::Vector2f(AC_WINDOW_X, AC_WINDOW_Y));
        }

        /**
         * @brief Update function for camera
         * 
         * @param window    sf::RenderWindow
         * @param pos       camera position
         * @param zoom      camera zoom
         * @param dt        delta time
         */
        void OnUpdate(sf::RenderWindow &window, v2f pos, f32 zoom, f32 dt) {
            // ----- Smoothening in x & y
            //sf::Vector2f movement = sf::Vector2f(pos.x, pos.y) - m_View.getCenter();
            //m_View.move(movement * dt * m_SmoothSpeed);
            
            // ----- No smoothening
            m_View.setCenter(sf::Vector2(pos.x, pos.y));
            m_View.setSize(sf::Vector2f(window.getSize().x * zoom, window.getSize().y * zoom));
            window.setView(m_View);
        }

    private:

        // ** Members **
        sf::View    m_View;             // Current view

        const f32 m_SmoothSpeed = 5;    // smoothening speed
    };
}