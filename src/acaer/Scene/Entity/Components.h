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
     * @brief This namespace contains custom userData
     *        For Box2D
     */
    namespace UserData {
        struct SensorUserData {
            UUID uuid;                          // tag (name)
            std::string name;                   // uuid of entity (64bit)
            b8 isColliding = false;
        };
    }

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


        struct SpriteAnimatior {
            struct Animation {
                u16 framePos     = 0;           // Animation pos on spritesheet
                u16 frameLenght  = 8;           // How many frames in animation
                v2<u16> frameSize;              // Size (with / height) of 1 frame

                b8 isMirrored   = false;        // Mirror animation

                u16 currentFrame = 0;;
                f32 animationSpeed = 10.f;      // animationSpeed [fps]
                f32 elapsedTime = 0.f;
            };

            std::string currentAnimation;
            std::map<std::string, Animation> pool;
        };

        /**
         * @brief Sprite Animation Component
         * 
         */
        struct Sprite {
            // TODO (flex): runtime texture reload
            std::string texturepath;            // Texturepath
            sf::Sprite spriteTexture;           // sf::Sprite
        };

        /**
         * @brief Transform Component
         *  
         */
        struct Transform {
            //v2 center_pos;
            s8 renderLayer = 0;                // Order in the renderer [0..AC_MAX_RENDERLAYERS]
            
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
                UserData::SensorUserData* userData = nullptr;

                v2f size   = {10, 10};              // Size [px]
                v2f offset = {0, 0};                // Offset [px] relative to center of collider.
            
                Sensor() { userData = new UserData::SensorUserData(); }
            };

            v2f size   = {10, 10};                  // Size [px]
            v2f offset = {0, 0};                    // Offset [px] relative to pos of Transform. usually {0, 0}
            std::map<std::string, Sensor> sensors;  // Map for sensors
        };

        /**
         * @brief CameraController Component.
         * @attention There may only be one per Scene
         * 
         */
        struct CameraController {
            f32 zoom = 1.0f;                    // Camera zoom
        };

        /**
         * @brief Nativ Script component
         * 
         * @note  Cpp Compiled Script
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

        /**
         * @brief Script componnent
         * 
         * @note Lua Runtime Script
         */
        struct Script {
            std::vector<std::string> pool;      // Filepath script pool
        };
    };
}
