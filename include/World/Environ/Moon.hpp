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

#ifndef _WEATHER_MOON_HPP_
#define _WEATHER_MOON_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"

#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "World/Environ/Object.hpp"

#include <math.h>

class Moon : public EnvironObject
{
    private:
        // Moon graphic representing
        CL_Image mMoon;
        CL_Colorf mMoonColor;
        float mG, mB;

        // Moon ellipse orbit
        int mRadius;
        float mCenterX, mCenterY;
        float mAngle; // The angle where Moon is currently located
        float t;  // For the moon color&size transation

        bool mRenderMoon;

        CL_GraphicContext mGC;

    public:
        Moon();
        void setScale(float _scaleX, float _scaleY);
        float getMoonAngle();
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_MOON_HPP_ */
