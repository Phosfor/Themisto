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

#ifndef _OBJECT_MOON_HPP_
#define _OBJECT_MOON_HPP_

#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "World/Objects/Object.hpp"
#include "Core/EnvironManager.hpp"

#include <math.h>

class Moon : public Object
{
    private:
        // Moon graphic representing
        CL_Image mMoon;
        CL_Colorf mMoonColor;
        float mG, mB;

        float mPosX, mPosY;

        // Moon ellipse orbit
        uint16_t mRadius;
        float mCenterX, mCenterY;
        float mAngle; // The angle where Moon is currently located
        float t;  // For the moon color&size transation

        bool mRenderMoon;

        CL_GraphicContext mGC;

    public:
        Moon();
        ~Moon();

        void setScale(float _scaleX, float _scaleY);
        float getMoonAngle();
        void setMoonAngle(float angle);

        float getOrbitRadius();
        void setOrbitRadius(float radius);

        void setVisual(std::string textureName, std::string textureSection, float width, float height);
        void setVisual(std::string textureName, std::string textureSection);

        // Parser
        static boost::shared_ptr<Object> ParseMoon(CL_DomElement *node, std::string &desc);

        // --- Object implementation ---
        void init();
        void updateVisual(float newX, float newY);
        void update(float elapsed);

        CL_Pointf getPosition();
        void setPosition(CL_Pointf newPos);
        CL_Rectf getRectangle();
};

#endif /* _OBJECT_MOON_HPP_ */
