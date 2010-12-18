/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

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
#include "World/Objects/Object.hpp"
#include "Physic/BodyPart.hpp"
#include <iostream>
#include <list>

#include "Core/PhysicManager.hpp"
#include "Core/EnvironManager.hpp"
#include "Core/ObjectsManager.hpp"
#include "World/Environ/Types.hpp"
#include <boost/lexical_cast.hpp>

class Body
{
    private:
        b2AABB mLastLocation;
        b2AABB calculateLocation();
        std::string mName;

        // 0 - body will sink, more - stronger force would pop body from water
        float mBuoyancy;

    protected:

        b2Body *mBody;
        b2World *mParentWorld;
        WorldManager* world;


    public:
        Body(b2Body* body);
        ~Body();

        // Get/set 

        // TODO: Move implementation into cpp!
        b2Body* getb2Body();
        float getBuoyancy() { return mBuoyancy; }
        void setBuoyancy(float value) { mBuoyancy = value; }
        std::string getName() { return mName; }
        void setName(const std::string &name) { mName = name; }

        void step(float32 elapsed);

        // Should body free memory under mBody object at destroing
        // Default is true
        bool mShouldFreeB2Body; 
};

#endif /* _PHYSIC_BODY_HPP_ */
