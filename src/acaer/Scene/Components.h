/**
 * @file Components.h
 * @author flaxhaufen
 * @brief Components for Entites
 * @version 0.1
 * @date 2023-02-07
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


    struct Sprite_C {
        Texture2D sprite_texture;           // Sprite Texutre    
    };

    struct Transform_C {
        std::string tag;                    // tag (name)

        //v2 center_pos;
        s8 render_layer;                    // Order in the renderer
        
        // Debug rec
        Rectangle rec = {50, 50, 50, 50};   // Rectangle Transform
        Color color = {200, 0, 200, 255};   // Color of transform   default: Pink #C800C8
    };

    struct RigidBody_C {
        f32 mass;                           // Mass of entity
        f32 inv_mass = 0.f;                 // Inverse mass for easy calculations
        b8  isMovable;                      // inf. mass

        f32 restitution;      
        f32 drag;                           // Decay rate of liniar velocity
        // f32 angularDrag;                 // Rotational decay rate

        b8  usesGravity;                    // true: affectet by gravity
        f32 gravityFactor;                  // gravity multiplyer factor 

        b8  isPassible;

        // privat
        v2<float> velocity;

    };

    struct Collider_C {
    };

  
    // *** Input ***
    struct Input_C  {
        b8 isControllable = true;           // true: Player is controllable via input
    };

};
