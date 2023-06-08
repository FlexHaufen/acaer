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
            b2BodyDef bodyDef;

            bodyDef.type = (b2BodyType)rb.type; // NOTE: Type conversion is possible because of same order
            bodyDef.position.Set(t.pos.x / AC_PPM, t.pos.y / AC_PPM);
            bodyDef.angle = t.rotation / AC_DEG_PER_RAD;

            b2Body* body = world->CreateBody(&bodyDef);
            body->SetFixedRotation(rb.fixedRoation);
            rb.RuntimeBody = body;

            
            
            // ** main fixture **
            b2FixtureDef fixtureDef;
            b2PolygonShape polyShape;
            polyShape.SetAsBox((c.size.x / 2.f / AC_PPM), (c.size.y / 2.f / AC_PPM)); // TODO: Add ability to don't use scale 
            fixtureDef.shape        = &polyShape;
            fixtureDef.density      = rb.density;
            fixtureDef.restitution  = rb.restitution;
            fixtureDef.friction     = rb.friction;
            fixtureDef.restitutionThreshold = rb.restitutionThreshold;
            body->CreateFixture(&fixtureDef);

            for (auto i : c.sensors) {
                b2FixtureDef fixtureDefSensor;
                b2PolygonShape polyShapeSensor;
                polyShapeSensor.SetAsBox((i.size.x / 2.f / AC_PPM), (i.size.y / 2.f / AC_PPM)); // TODO: Add ability to don't use scale
                fixtureDefSensor.shape      = &polyShapeSensor;
                fixtureDefSensor.isSensor   = true;

                // TODO: Fix UserData
                // NOTE: No idea what this does and if it works - need to check
                // https://gamedev.stackexchange.com/questions/196951/how-do-i-correctly-use-userdata-in-box2d
                fixtureDefSensor.userData.pointer  = reinterpret_cast<uintptr_t>(&i.id);
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