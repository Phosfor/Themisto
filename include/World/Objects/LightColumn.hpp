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

#ifndef _LIGHTCOLUMN_OBJECT_HPP_
#define _LIGHTCOLUMN_OBJECT_HPP_

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <ClanLib/core.h>

#include "World/WorldManager.hpp"
#include "World/Objects/Object.hpp"
#include "Core/Utils.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/InputManager.hpp"

class LightColumn: public Object
{
    private:
        // Visual
        CL_Sprite mImageHandle, mLighting, mBug;
        CL_GraphicContext mGC;

        CL_Pointf mPos;
        CL_Rectf mBoundingBox;

        // For bugs
        float mAbsTime;
        float x, y, x_, y_, x__, y__;

    public:
        explicit LightColumn();
        ~LightColumn();

        // Get/set
        void setVisual(std::string textureName, std::string textureSection, float width, float height);
        void setVisual(std::string textureName, std::string textureSection);

        void updateVisual();

        // Parsing object
        static boost::shared_ptr<Object> ParseLightColumn(CL_DomElement* node, std::string &desc);

        // --- Object implementation ---
        CL_Pointf getPosition();
        void setPosition(CL_Pointf newPos);
        void update(float elapsed);
        void updateVisual(float newX, float newY);
        CL_Rectf getRectangle();
};

#endif /* _LIGHTCOLUMN_OBJECT_HPP_ */
