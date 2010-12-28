/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
