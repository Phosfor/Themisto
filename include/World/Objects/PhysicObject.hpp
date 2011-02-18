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

//#include "World/Actions/TakeAction.hpp"

class Body;
class Impact;


class PhysicObject: public Object
{
    private:
        boost::shared_ptr<Body> mBody;
        std::vector< boost::shared_ptr<Action> > mActions;

        // Visual
        CL_Sprite mImageHandle;
        CL_GraphicContext mGC;

    public:
        explicit PhysicObject();

        // Get/set
        boost::shared_ptr<Body> getBody();
        void setVisual(std::string textureName, std::string textureSection, float width, float height);
        void setVisual(std::string textureName, std::string textureSection);

        void updateVisual();
        void step(float32 elapsed); // Physic

        // Parsing object
        static boost::shared_ptr<Object> ParsePhysicObject(CL_DomElement* node, std::string &desc);

        // --- Object implementation ---
        CL_Pointf getPosition();
        void setPosition(CL_Pointf newPos);
        void update(float elapsed);
        void updateVisual(float newX, float newY);
        CL_Rectf getRectangle();
        std::vector< boost::shared_ptr<Action> > getAvailableActions();
};

#endif /* _PHYSIC_OBJECT_HPP_ */
