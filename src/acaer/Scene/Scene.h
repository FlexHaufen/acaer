/**
 * @file Scene.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-02-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
# pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"

//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {
    class Entity;

    class Scene {

    public:
        Scene();
        ~Scene();

        /**
         * @brief Create a new Entity
         * 
         * @return Entity 
         */
        Entity CreateEntity();

        /**
         * @brief Update function
         * 
         * @param dt delta time
         */
        void OnUpdate(f32 dt);

    private:

        // ** Members **
        entt::registry m_Registry;

        friend class Entity;
    };
};