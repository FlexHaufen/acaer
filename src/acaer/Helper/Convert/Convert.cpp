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
            
            
            // TODO: reconsider Box2d body definition with sensors,
            //       main fixture, etc.
            
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
            body->CreateFixture(&mainFixture);

            // ** Sensors **
            for (auto i : c.sensors) {

                b2PolygonShape polyShapeSensor;
                v2f sensorSize = {(i.size.x / 2.f / AC_PPM), (i.size.y / 2.f / AC_PPM)};
                b2Vec2 offsetPosition((i.offset.x / 2.f / AC_PPM), (i.offset.x / 2.f / AC_PPM));
                polyShapeSensor.SetAsBox(sensorSize.x, sensorSize.y, offsetPosition, 0); // TODO: Add ability to don't use scale
                
                b2FixtureDef sensorFixture;
                sensorFixture.shape      = &polyShapeSensor;
                sensorFixture.isSensor   = true;

                // TODO: Fix UserData
                // NOTE: No idea what this does and if it works - need to check
                // https://gamedev.stackexchange.com/questions/196951/how-do-i-correctly-use-userdata-in-box2d
                sensorFixture.userData.pointer  = reinterpret_cast<uintptr_t>(&i.userData);

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
    }
}