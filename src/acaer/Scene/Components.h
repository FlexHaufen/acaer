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
#include "acaer/Helper/UUID/UUID.h"

//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    struct Tag_C {
        std::string tag = "";               // tag (name)

        UUID uuid;                          // uuid of entity (64bit)
    
        Tag_C() = default;
		Tag_C(const Tag_C&) = default;
    };

    /**
     * @brief Sprite Component
     * 
     */
    struct Sprite_C {
        Texture2D sprite_texture;           // Sprite Texutre    
    };

    /**
     * @brief Transform Component
     * @note  If the "alpha" of ".color" is set to "0",
     *        the transform won't be rendered
     * 
     */
    struct Transform_C {
        //v2 center_pos;
        s8 render_layer = 0;                // Order in the renderer
        
        // Debug rec
        Rectangle rec = {50, 50, 50, 50};   // Rectangle Transform
        Color color = {200, 0, 200, 255};   // Color of transform   default: Pink #C800C8
    };

    /**
     * @brief RigitBody Component
     * 
     */
    struct RigidBody_C {
        f32 mass;                           // Mass of entity
        f32 inv_mass = 0.f;                 // Inverse mass for easy calculations
        b8  isMovable;                      // inf. mass

        f32 restitution;      
        f32 drag;                           // Decay rate of liniar velocity
        // f32 angularDrag;                 // Rotational decay rate

        b8  usesGravity;                    // true: affectet by gravity
        f32 gravityFactor;                  // gravity multiplyer factor 


        // privat
        v2<float> velocity;                 // velocity x / y

    };

    /**
     * @brief Collision Component.
     * 
     */
    struct Collider_C {
        b8  isPassible;                     // true: collider is passible
        Rectangle rec {0, 0, 0, 0,};        // Rectangle Collider
    };

    /**
     * @brief Camera Component.
     * @attention There may only be one per Scene
     * 
     */
    struct Camera_C {
        f32 zoom = 1.0f;                    // Camera zoom
    };

    /**
     * @brief Input Component.
     *        Entities with this commponent can be moved 
     *        by the player
     */
    struct Input_C  {
        b8 isControllable = true;           // true: Player is controllable via input
    };
}