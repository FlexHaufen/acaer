/**
 * @file ContactListener.h
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-06-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Scene/Entity/Components.h"

//*** DEFINES ***


//*** NAMESPACE ***
namespace Acaer {

    /**
     * @brief ContactListener 
     * 
     */
    class ContactListener : public b2ContactListener {
        void BeginContact(b2Contact* contact) override {
            b2Fixture* fixtureA = contact->GetFixtureA();
            b2Fixture* fixtureB = contact->GetFixtureB();

            // Check if either fixture is a sensor
            if (fixtureA->IsSensor() || fixtureB->IsSensor()) {
                // Get the user data of the sensor fixture
                uintptr_t sensorUserData = (fixtureA->IsSensor()) ? fixtureA->GetUserData().pointer : fixtureB->GetUserData().pointer;
                // Cast the user data back to its original type
                UserData::SensorUserData* userData = reinterpret_cast<UserData::SensorUserData*>(sensorUserData);
                userData->isColliding = true;
            }
        }

        void EndContact(b2Contact* contact) {
            b2Fixture* fixtureA = contact->GetFixtureA();
            b2Fixture* fixtureB = contact->GetFixtureB();
        
            // Check if either fixture is a sensor
            if (fixtureA->IsSensor() || fixtureB->IsSensor()) {
                // Get the user data of the sensor fixture
                uintptr_t sensorUserData = (fixtureA->IsSensor()) ? fixtureA->GetUserData().pointer : fixtureB->GetUserData().pointer;
                // Cast the user data back to its original type
                UserData::SensorUserData* userData = reinterpret_cast<UserData::SensorUserData*>(sensorUserData);
                userData->isColliding = false;
            }
        }
    };
}