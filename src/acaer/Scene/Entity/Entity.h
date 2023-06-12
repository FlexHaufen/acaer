/**
 * @file Entity.h
 * @author flexhaufen
 * @brief entity class
 * @version 0.1
 * @date 2023-02-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Scene.h"
#include "acaer/Helper/UUID/UUID.h"

//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handel, Scene* scene) : m_EntityHandle(handel), m_Scene(scene) {}
        Entity(const Entity& other) = default;

        /**
         * @brief           Adds component to entity 
         * 
         * @tparam T        Type of component to create.
         * @tparam Args     Types of arguments to use to construct the component.
         * @param args      Parameters to use to initialize the component.
         * @return T&       A reference to the newly created component.
         */
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) { 
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...); 
        }

        /**
         * @brief           Gets component of entity 
         * 
         * @tparam T        Types of components to get.
         * @return T&       References to the components owned by the entity.
         */
        template<typename T>
        T& GetComponent() { 
            return m_Scene->m_Registry.get<T>(m_EntityHandle); 
        }

        /**
         * @brief           Gets or emplaces component of entity 
         * 
         * @tparam T        Types of components to get.
         * @return T&       References to the components owned by the entity.
         */
        template<typename T>
        T& GetOrEmplaceComponent() { 
            return m_Scene->m_Registry.get_or_emplace<T>(m_EntityHandle); 
        }

        /**
         * @brief           Checks if entity has the given components
         * 
         * @tparam T        Types of components to get.
         * @return True     Entity has given components
         */
        template<typename T>
        bool HasComponent() { 
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        /**
         * @brief           Removes component from entity if exists
         * 
         * @tparam T        Types of components to remove.
         */
        template<typename T>
        void RemoveComponent() {
            return m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        /**
         * @brief Gets UUID of entity
         * 
         * @return UUID 
         */
        UUID GetUUID() { return GetComponent<Component::Tag>().uuid; }

        /**
         * @brief Get the tag of entity
         * 
         * @return const std::string& tag
         */
		const std::string& GetTag() { return GetComponent<Component::Tag>().tag; }


        // *** Custom operators ***
        operator b8() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator u32() const { return (uint32_t)m_EntityHandle; }

        b8 operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        b8 operator!=(const Entity& other) const { return !(*this == other); }

    private:

        // *** Members ***
        entt::entity m_EntityHandle = { entt::null };   // entity
        Scene* m_Scene = nullptr;                       // scene reference
    };
}