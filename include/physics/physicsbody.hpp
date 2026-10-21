#pragma once

// Describes a Box2D physics body that can be linked to an entity
#include <box2d/box2d.h>

namespace sg::Physics {

    class PhysicsBody {

        b2BodyDef bodyDef;
        b2BodyId bodyId;
        // b2Polygon bodyPoly;
        b2ShapeDef bodyShapeDef;
        b2WorldId worldId;
        bool is_static = false; // is this body static?
        void CreateBody(bool isStatic, b2Vec2 position, b2WorldId world);
    public:

        const b2BodyId& GetBodyId() { return this->bodyId; }

        PhysicsBody(bool isStatic, b2Vec2 pos, b2WorldId worldId, const b2Polygon& polygon);
        PhysicsBody(bool isStatic, b2Vec2 pos, b2WorldId worldId, const b2Circle& circle);

        const b2BodyDef& GetBodyDef() { return this->bodyDef; }
        // const b2Polygon& GetBodyPolygon() { return this->bodyPoly; }
        const b2ShapeDef& GetShapeDef() { return this->bodyShapeDef; }
        const b2WorldId& GetWorldId() { return this->worldId; }
        const bool IsStatic() { return is_static; }

    };

};