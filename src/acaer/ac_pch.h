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
#include <unordered_map>
#include <functional>

// ** External **
// * SFML *
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

// * Box2D *
#include "box2d/box2d.h"

// * EnTT *
#include "entt.hpp"

// * yaml-cpp *
#pragma warning(push, 0)
    #include "yaml-cpp/yaml.h"
#pragma warning(pop)

// * ImGui *
//#include "imgui.h"

// ** Local **
#include "acaer/Helper/Types.h"
#include "acaer/Core/Log.h"


// *** DEFINES ***
#define AC_VERSION                          "0.0.1"


#define AC_WINDOW_RESIZABLE                             // flag
#define AC_WINDOW_X                         800
#define AC_WINDOW_Y                         450
#define AC_CALC_FPS

#define AC_SCENE_CLEAR_BACKGROUND           sf::Color::Black

//#define AC_SCENE_LOAD_ON_OPEN                           // flag
#define AC_SCENE_SAVE_ON_CLOSE                          // flag

#define AC_RENDER_ENTITY_HITBOX                         // flag
#define AC_RENDER_ENTITY_HITBOX_THICKNESS   1    
//#define AC_RENDER_ENTITY_REC                            // flag


#define AC_PHYSICS_VEL_STEPS               10
#define AC_PHYSICS_POS_STEPS                6
#define AC_PPM                             30           // pixels per meter     [SFML -> BOX2D]
#define AC_DEG_PER_RAD                     57.295795F   // degrees per radian   [SFML -> BOX2D]