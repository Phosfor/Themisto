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

#pragma once

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/LevelManager.hpp"
#include "World/Objects/Object.hpp"

class Body;
class Level : public Object
{
    protected:
        boost::shared_ptr<Body> mBody;

        CL_Sprite mImageHandle;
        CL_GraphicContext mGC;

        std::string mTexture, mSection;

    public:
        Level();

        // Get/set
        void setVisual(std::string textureName, std::string textureSection, float width, float height);
        void setVisual(std::string textureName, std::string textureSection);

        static boost::shared_ptr<Object> ParseLevel(CL_DomElement *tag, std::string &desc);

        std::vector<std::string> getTextureInfo();
        boost::shared_ptr<Body> getBody();

        // Object implementation
        bool checkCollision(CL_Pointf point);
        CL_Pointf getPosition();
        void setPosition(CL_Pointf newPos);
        CL_Rectf getRectangle();

        void init();
        void update(float elapsed);
        void updateVisual(float newX, float newY);
};
