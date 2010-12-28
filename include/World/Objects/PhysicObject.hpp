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

#ifndef _PHYSIC_OBJECT_HPP_
#define _PHYSIC_OBJECT_HPP_

#include <Box2D/Box2D.h>
#include <ClanLib/core.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>

#include "Physic/BodyMaterial.hpp"

#include "Physic/Body.hpp"
#include "Physic/Impact.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/BodyVisual.hpp"
#include "World/WorldManager.hpp"
#include "Physic/AreaManager.hpp"
#include "World/Objects/Object.hpp"
#include "Physic/BodyPart.hpp"

class PhysicObject: public Object
{
    private:
        BodyVisual *mBodyVisual;
        Body* mBody;

    public:
        PhysicObject(Body* body, BodyVisual* visual);
        ~PhysicObject();

        // Get/set 
        void setVisual(BodyVisual* visualiser);
        BodyVisual& getVisual();
        void setBody(Body* body);
        Body& getBody();

        void updateVisual();
        void step(float32 elapsed); // Physic

        // Should body free memory under mBodyVisual object at destroing
        // Default value is true
        bool mShouldFreeBodyVisual;

        // Should body free memory under mBody object at destroing
        // Default is true
        bool mShouldFreeBody; 

        // Parsing object
        static Object* ParsePhysicObject(CL_DomElement* node, std::string &desc);

        // --- Object implementation ---
        CL_Pointf getPosition();
        void update(float elapsed);
};

#endif /* _PHYSIC_OBJECT_HPP_ */
