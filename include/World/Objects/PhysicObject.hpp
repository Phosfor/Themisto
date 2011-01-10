/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

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
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <ClanLib/core.h>

#include "World/WorldManager.hpp"
#include "World/Objects/Object.hpp"
#include "Core/Utils.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ApplicationManager.hpp"

class BodyVisual;
class Body;
class BodyPart;
class BodyState;
class Impact;
class BodyMaterial;

class PhysicObject: public Object
{
    private:
        std::vector< boost::shared_ptr<BodyPart> > mParts;
        b2Body *mBody;

        // Visual
        CL_Sprite mImageHandle;
        CL_GraphicContext mGC;

    public:
        explicit PhysicObject();
        ~PhysicObject();

        // Get/set
        std::vector< boost::shared_ptr<BodyPart> > getParts();
        void setParts(std::vector< boost::shared_ptr<BodyPart> > parts);
        b2Body* getb2Body();
        void setb2Body(b2Body* body);
        void setVisual(std::string textureName, std::string textureSection, float width, float height);
        void setVisual(std::string textureName, std::string textureSection);

        void updateVisual();
        void step(float32 elapsed); // Physic

        // Parsing object
        static boost::shared_ptr<Object> ParsePhysicObject(CL_DomElement* node, std::string &desc);

        // --- Object implementation ---
        CL_Pointf getPosition();
        void update(float elapsed);
        void updateVisual(float newX, float newY);
        CL_Rectf getRectangle();

        // Should body free memory under mBody object at destroing
        // Default is true
        bool mShouldFreeB2Body;
};

#endif /* _PHYSIC_OBJECT_HPP_ */
