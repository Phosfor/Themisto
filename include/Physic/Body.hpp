#ifndef _PHYSIC_BODY_HPP_
#define _PHYSIC_BODY_HPP_

#include <Box2D/Box2D.h>
#include <ClanLib/core.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>

#include "Physic/BodyMaterial.hpp"
#include "Physic/Impact.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/BodyVisual.hpp"
#include "World/WorldManager.hpp"
#include "Physic/AreaManager.hpp"
#include "Physic/BodyPart.hpp"
#include <iostream>
#include <list>

class Body
{
    private:
        BodyVisual *mBodyVisual;
        b2AABB mLastLocation;
        b2AABB calculateLocation();

    protected:

        b2Body *mBody;
        b2World *mParentWorld;
        WorldManager* world;


    public:
        Body(b2Body* body);
        ~Body();

        void setVisual(BodyVisual* visualiser);
        b2Body* getb2Body();

        void updateVisual();
        void step(float32 elapsed);

        // Should body free memory under mBodyVisual object at destroing
        // Default value is true
        bool mShouldFreeBodyVisual;

        // Should body free memory under mBody object at destroing
        // Default is true
        bool mShouldFreeB2Body;

        // 0 - body will sink, more - stronger force will pop body from water
        float mBuoyancy;

};


#endif
