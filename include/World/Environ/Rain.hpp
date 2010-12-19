/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _WEATHER_RAIN_HPP_
#define _WEATHER_RAIN_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"
#include "World/Environ/Object.hpp"

const float kTail = 0.025f;
const float kDropAlpha = 0.5f;

struct Data
{
    float x, y, x_speed, y_speed;
    int timeout;
    Data();
};

class Rain : public EnvironObject
{
    private:
        std::vector<Data> mData;
        CL_Colorf mDropColor;
        CL_GraphicContext mGC;
        bool mFirstTime;

        void processDrops(float windPower, int i);

    public:
        Rain(int maxDrops = 150);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_RAIN_HPP_ */
