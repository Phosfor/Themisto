/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
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
