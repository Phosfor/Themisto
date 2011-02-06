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

struct LightBug
{
    int boundingValue;
    CL_Sprite imageHandle;
    int koef1, koef2;

    float x, y, offsetX, offsetY, theta;

    LightBug() : boundingValue(10), koef1(1), koef2(3), x(0), y(0), 
        offsetX(0), offsetY(0), theta((float)(rand()%10 + 5)/10.0f) { }
};

class LightColumn: public Object
{
    private:
        // Visual
        // Main lightpost image
        CL_Sprite mImageHandle;
        // Lightings images
        std::vector<CL_Sprite> mLightings;

        CL_GraphicContext mGC;

        // Position of main lightpost image
        CL_Pointf mPos;
        // Position of each lighting image
        std::vector<CL_Pointf> mLightingsPos;

        std::vector<LightBug> mBugs;

        CL_Rectf mBoundingBox;

        // For bugs
        float mAbsTime;

    public:
        explicit LightColumn();
        ~LightColumn();

        // Get/set
        void setVisual(std::string textureName, std::string textureSection, float width, float height);
        void setVisual(std::string textureName, std::string textureSection);

        void addLighting(const std::string &textureName, const std::string &textureSection, float width, float height);
        void setLightingPos(CL_Pointf pos);

        void addBug(LightBug &bug);

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
