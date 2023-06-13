/**
 * @file Convert.cpp
 * @author flexhaufen
 * @brief 
 * @version 0.1
 * @date 2023-03-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// *** INCLUDES ***
#include "acaer/ac_pch.h"
#include "acaer/Helper/Convert/Convert.h"

// *** NAMESPACE ***
namespace Acaer {
    namespace Convert {

        void create_b2Body(Component::RigidBody &rb,Component::Transform &t, Component::Collider &c, b2World *world) {
            
            // ** Box2d Body definition **
            b2BodyDef bodyDef;
            bodyDef.type = (b2BodyType)rb.type;                         // NOTE: Type conversion is possible because of same order
            bodyDef.position.Set(t.pos.x / AC_PPM, t.pos.y / AC_PPM);   // Position
            bodyDef.angle = t.rotation / AC_DEG_PER_RAD;                // Angle

            b2Body* body = world->CreateBody(&bodyDef);                 // Create body in world
            body->SetFixedRotation(rb.fixedRoation);                    // Fix rotation
            rb.RuntimeBody = body;

            
            // ** Main FixtureDef **
            b2PolygonShape polyShape;
            polyShape.SetAsBox((c.size.x / 2.f / AC_PPM), (c.size.y / 2.f / AC_PPM)); // TODO: Add ability to don't use scale 

            b2FixtureDef mainFixture;
            mainFixture.shape        = &polyShape;
            mainFixture.density      = rb.density;
            mainFixture.restitution  = rb.restitution;
            mainFixture.friction     = rb.friction;
            mainFixture.restitutionThreshold = rb.restitutionThreshold;
            
            // TODO: Creat UserData for mainFixture
            // Creating userdata
            //UserData::FixtureUserData* userData = new UserData::FixtureUserData;
            //userData->name = tag.name;
            //mainFixture.userData.pointer = reinterpret_cast<uintptr_t>(userData);
            body->CreateFixture(&mainFixture);

            // ** Sensors **
            for (auto [key, val] : c.sensors) {

                b2PolygonShape polyShapeSensor;
                v2f sensorSize = {(val.size.x / 2.f / AC_PPM), (val.size.y / 2.f / AC_PPM)};
                b2Vec2 offsetPosition((val.offset.x / 2.f / AC_PPM), (val.offset.y / 2.f / AC_PPM));
                polyShapeSensor.SetAsBox(sensorSize.x, sensorSize.y, offsetPosition, 0);
                
                b2FixtureDef sensorFixture;
                sensorFixture.shape      = &polyShapeSensor;
                sensorFixture.isSensor   = true;

                // Creating userdata
                sensorFixture.userData.pointer = reinterpret_cast<uintptr_t>(val.userData);
                body->CreateFixture(&sensorFixture);
            }
        }


        v2f getPositionFrom_b2Body(b2Body* body, Component::Collider c) {
            v2f pos = {(body->GetPosition().x * AC_PPM) - c.size.x / 2 + c.offset.x , 
                       (body->GetPosition().y * AC_PPM) - c.size.y / 2 + c.offset.y};
            return pos;
        }
    
        f32 getRotationFrom_b2Body(b2Body* body) {
            return (body->GetAngle() * AC_DEG_PER_RAD * -1);
        }

        vColor sfColor_to_vColor(sf::Color c) {
            return vColor({c.r, c.g, c.b, c.a}); //rgba
        }

        sf::Color vColor_to_sfColor(vColor c) {
            return sf::Color(c.r, c.g, c.b, c.a); //rgba
        }

        sf::Color b2Color_to_sfColor(b2Color c) {
            return sf::Color((u8)(c.r*255), (u8)(c.g*255), (u8)(c.b*255), (u8)(c.a*255));
        }
    }
}