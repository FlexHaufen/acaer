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
# pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Scene.h"

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
         * @brief           Removes component from entity if exists
         * 
         * @tparam T        Types of components to remove.
         */
        template<typename T>
        void RemoveComponent() {
            return m_Scene->m_Registry.remove_if_exists<T>(m_EntityHandle)
        };

        bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        bool operator!=(const Entity& other) const { return !(*this == other); }

    private:

        // *** Members ***
        entt::entity m_EntityHandle = { entt::null };
        Scene* m_Scene = nullptr;
    };



};