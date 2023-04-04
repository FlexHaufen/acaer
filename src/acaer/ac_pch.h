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

#pragma warning(push, 0)
#include <algorithm>
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
#include <box2d/box2d.h>

// * EnTT *
#include "entt.hpp"

// * yaml-cpp *
#include "yaml-cpp/yaml.h"

// * ImGui *
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui-SFML.h"
#include "imgui.h"



#pragma warning(pop)


// ** Local **
#include "acaer/Helper/Types.h"
#include "acaer/Core/Log.h"


// *** DEFINES ***
#define AC_VERSION                          "0.0.3a"


#define AC_WINDOW_RESIZABLE                             // flag
#define AC_WINDOW_X                         1920
#define AC_WINDOW_Y                         1080
#define AC_CALC_FPS
#define AC_WINDOW_ICON_PATH                 "resource/images/acaer_logo.png"

#define AC_SCENE_CLEAR_BACKGROUND           sf::Color(158, 222, 232, 255)   // light blue

#define AC_GLOBAL_SCALE                     4          
#define AC_RENDER_ENTITY_HITBOX                         // flag
#define AC_RENDER_ENTITY_HITBOX_THICKNESS   1    
//#define AC_RENDER_ENTITY_REC                            // flag

// Converion
#define AC_PPM                             30           // pixels per meter     [SFML -> BOX2D]
#define AC_DEG_PER_RAD                     57.295795F   // degrees per radian   [SFML -> BOX2D]

// Physics
#define AC_GRAVITY_X                        0           // Gravity in x-direction
#define AC_GRAVITY_Y                       40           // Gravity in y-direction
#define AC_PHYSICS_VEL_STEPS               10
#define AC_PHYSICS_POS_STEPS                6

// World
#define AC_WORLD_CHUNCK_SIZE               64           // x / y chunck size [AC_PX]