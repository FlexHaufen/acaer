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

// ** External **
// * Raylib *
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>

// * EnTT *
#include "entt.hpp"

#include <string>

// ** Local **
#include "Helper/Types.h"


// *** DEFINES ***


// ** Flags **
#define AC_WINDOW_RESIZABLE
#define AC_SCENE_CLEAR_BACKGROUND           DARKGRAY

#define AC_RENDER_ENTITY_HITBOX             // flag

#define AC_RENDER_ENTITY_TAG                // flag
#define AC_RENDER_ENTITY_TAG_FONT_COLOR     GREEN
#define AC_RENDER_ENTITY_TAG_FONT_SIZE      20
