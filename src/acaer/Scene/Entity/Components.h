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


    class ScriptableEntity; // Forward declaration

    /**
     * @brief This namespace contains all components
     *        that an entity can have
     */
    namespace Component {

        /**
         * @brief Tag Component
         * 
         */
        struct Tag {
            std::string tag = "";               // tag (name)

            UUID uuid;                          // uuid of entity (64bit)
        
            Tag() = default;
            Tag(const Tag&) = default;
        };

        /**
         * @brief Sprite Component
         * 
         */
        struct Sprite {
            sf::Texture texture;                // sf::RenderTexture
        };

        /**
         * @brief Transform Component
         * @note  If the "alpha" of ".color" is set to "0",
         *        the transform won't be rendered
         * 
         */
        struct Transform {
            //v2 center_pos;
            s8 render_layer = 0;                // Order in the renderer [0..9]
            
            v2f pos =  {10, 10};                // Position [px]
            v2f scale =  {1, 1};                // Scale [1].

            f32 rotation = 0.f;                 // Rotation

        };

        /**
         * @brief RigitBody Component
         * 
         */
        struct RigidBody {
            enum class BodyType { Static = 0, Kinematic, Dynamic};
            BodyType type = BodyType::Static;   // Type         [Static, Kinematic, Dynamic]

            b8 fixedRoation = true;             // Fixed rotation. On by default

            f32 density = 1.0f;                 // Density      [kg/m^2]
            f32 friction = 0.5f;                // Friction     [0..1]
            f32 restitution = 0.0f;             // Restitution  [0..1]
            f32 restitutionThreshold = 0.f;     // Restitution velocity threshold, [m/s]. 
                                                // apply restitution above this speed

            b2Body *RuntimeBody = nullptr;      // b2Body at runtime
        };

        
        /**
         * @brief Collider Component
         * 
         */
        struct Collider {
            
            /**
             * @brief Sensor Component
             * 
             */
            struct Sensor {
                std::string id = "";            // Id of sensor
                v2f size   = {10, 10};          // Size [px]
                v2f offset = {0, 0};            // Offset [px] relative to pos of Transform. usually {0, 0}
            };

            v2f size   = {10, 10};              // Size [px]
            v2f offset = {0, 0};                // Offset [px] relative to pos of Transform. usually {0, 0}
            std::vector<Sensor> sensors;        // Vector for sensors
        };

        /**
         * @brief Camera Component.
         * @attention There may only be one per Scene
         * 
         */
        struct Camera {
            f32 zoom = 1.0f;                    // Camera zoom
        };

        /**
         * @brief Input Component.
         *        Entities with this commponent can be moved 
         *        by the player
         */
        struct Input  {
            b8 isControllable = true;           // true: Player is controllable via input
        };

        /**
         * @brief Nativ Script component
         * 
         */    
        struct NativeScript {
            ScriptableEntity* Instance = nullptr;

            ScriptableEntity*(*InstantiateScript)();
            void (*DestroyScript)(NativeScript*);

            template<typename T>
            void Bind() {
                InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
                DestroyScript = [](NativeScript* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
            }
        };
    };
}
