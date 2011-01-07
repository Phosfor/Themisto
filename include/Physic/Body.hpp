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

#include <iostream>
#include <list>

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Box2D.h>
#include <ClanLib/core.h>

#include "World/WorldManager.hpp"
#include "Core/PhysicManager.hpp"
#include "Core/EnvironManager.hpp"
#include "Core/ObjectsManager.hpp"

#include "World/Environ/Types.hpp"

class Impact;
class BodyState;
class BodyVisual;
class Object;
class BodyPart;
class BodyMaterial;

class Body
{
    private:
        b2AABB mLastLocation;
        b2AABB calculateLocation();
        std::string mName;
        std::vector< boost::shared_ptr<BodyPart> > mParts;

    protected:

        b2Body *mBody;
        b2World *mParentWorld;
        WorldManager* world;

    public:
        explicit Body(b2Body* body);
        ~Body();

        // Get/set

        // TODO: Move implementation into cpp!
        b2Body* getb2Body();
        std::string getName();
        void setName(const std::string &name);
        std::vector< boost::shared_ptr<BodyPart> > getParts();

        void step(float32 elapsed);

        // Should body free memory under mBody object at destroing
        // Default is true
        bool mShouldFreeB2Body;
};

#endif /* _PHYSIC_BODY_HPP_ */
