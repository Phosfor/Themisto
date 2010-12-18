/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _WEATHER_CLOUDS_HPP_
#define _WEATHER_CLOUDS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <boost/foreach.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "World/Environ/Object.hpp"

struct CloudData
{
        float x, y_offset, x_speed, speed_koef;
        int cloudType, timeout;
        CL_Sprite imageHandle;
        CL_Colorf mColor;
};

class Clouds : public EnvironObject
{
    private:
        std::vector<CloudData> mClouds;
        CL_GraphicContext mGC;
        bool mFirstTime;

        void processClouds(CL_GraphicContext &gc, float windPower, int i);

    public:
        Clouds(int maxClouds = 4);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);

        // For the lightning
        CL_Pointf getCloudPos();
};

#endif /* _WEATHER_CLOUDS_HPP_ */
