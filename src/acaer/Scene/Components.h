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
        v2f size = {10, 10};
        v2f pos =  {10, 10};
        v2f scale =  {1, 1};

        f32 rotation = 0.f;

        vColor color = {200, 0, 200, 255};   // Color of transform   default: Pink #C800C8
    };

    /**
     * @brief RigitBody Component
     * 
     */
    struct RigidBody_C {
        enum class BodyType { Static = 0, Kinematic, Dynamic};
        BodyType type = BodyType::Static;   // Type

        b8 fixedRoation = true;

        f32 density = 1.0f;
        f32 friction = 0.5f;                // Decay rate of liniar velocity
        f32 restitution = 0.0f;      
        f32 restitutionThreshold = 0.5f;

        b2Body *RuntimeBody = nullptr;
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


	class ScriptableEntity; // Forward declaration
    /**
     * @brief Nativ Script component
     * 
     */    
	struct NativeScript_C {
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScript_C*);

		template<typename T>
		void Bind() {
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScript_C* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}
