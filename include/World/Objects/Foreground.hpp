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

#include "boost/cstdint.hpp"

class Foreground: public Object
{
    private:
        // Visual
        CL_Image mForeground;
        CL_GraphicContext mGC;

        float mYPos, mActualSize;
        bool mFixed;

        std::string mTexture, mSection;

    public:
        Foreground();

        // Get/set
        void setVisual(std::string textureName, std::string textureSection, float width, float height);
        void setVisual(std::string textureName, std::string textureSection);

        // Parsing object
        static boost::shared_ptr<Object> ParseForeground(CL_DomElement tag);

        void setFixed(bool state);
        bool getFixed();

        void setActualSize(uint16_t size);
        uint16_t getActualSize();

        std::vector<std::string> getTextureInfo();

        // --- Object implementation ---
        bool checkCollision(CL_Pointf point);
        CL_Pointf getPosition();
        void setPosition(CL_Pointf newPos);
        CL_Rectf getRectangle();

        void init();
        void update(float elapsed);
        void updateVisual(float newX, float newY);
};
