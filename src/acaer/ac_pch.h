/**
 * @file ac_pch.h
 * @author flexhaufen
 * @brief Precompiled Header
 * @version 0.1
 * @date 2023-02-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

// *** INCLUDES ***

#include <string>
#include <fstream>
#include <iostream>

// ** External **
// * Raylib *
// TODO: is there a way to namspace Raylib?
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <rcamera.h>

// * SFML *
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>


// * EnTT *
#include "entt.hpp"

// * yaml-cpp *
#pragma warning(push, 0)
    #include "yaml-cpp/yaml.h"
#pragma warning(pop)

// * ImGui *
#include "imgui.h"

// ** Local **
#include "Helper/Types.h"


// *** DEFINES ***


#define AC_WINDOW_RESIZABLE                             // flag
#define AC_WINDOW_X                         800
#define AC_WINDOW_Y                         450

#define AC_SCENE_CLEAR_BACKGROUND           sf::Color::Black    // Type = Raylib Color

#define AC_RENDER_ENTITY_HITBOX                         // flag
//#define AC_RENDER_ENTITY_REC                            // flag
