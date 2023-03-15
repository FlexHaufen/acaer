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

        void create_b2Body(RigidBody_C &rb,Transform_C &t, Collider_C &c, b2World *world) {
            b2BodyDef bodyDef;

            bodyDef.type = (b2BodyType)rb.type; // NOTE: Type conversion is possible because of same order
            bodyDef.position.Set(t.pos.x / AC_PPM, t.pos.y / AC_PPM);
            bodyDef.angle = t.rotation / AC_DEG_PER_RAD;

            b2Body* body = world->CreateBody(&bodyDef);
            body->SetFixedRotation(rb.fixedRoation);
            rb.RuntimeBody = body;

            
            b2PolygonShape polyShape;
            // TODO: Add ability to don't use scale 
            polyShape.SetAsBox((c.size.x / 2.f / AC_PPM), (c.size.y / 2.f / AC_PPM));
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape        = &polyShape;
            fixtureDef.density      = rb.density;
            fixtureDef.restitution  = rb.restitution;
            fixtureDef.friction     = rb.friction;
            fixtureDef.restitutionThreshold = rb.restitutionThreshold;

            body->CreateFixture(&fixtureDef);
        }


        v2f getPositionFrom_b2Body(b2Body* body, Collider_C c) {
            v2f pos = {(body->GetPosition().x * AC_PPM) - c.size.x / 2 + c.offset.x , 
                       (body->GetPosition().y * AC_PPM) - c.size.y / 2 + c.offset.y};
            return pos;
        }
    
        f32 getRotationFrom_b2Body(b2Body* body) {
            return (body->GetAngle() * AC_DEG_PER_RAD * -1);
        }
    }
}