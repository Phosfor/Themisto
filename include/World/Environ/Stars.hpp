/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _WEATHER_STARS_HPP_
#define _WEATHER_STARS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "World/Environ/Object.hpp"

struct StarsData
{
    float x, y, size, brightness;
    CL_Colorf color;

    StarsData(int width, int height);
};

struct ImageStarsData
{
    float x, y, brightness;
    CL_Image imageHandle;

    ImageStarsData(CL_GraphicContext gc, int width, int height);
};

class Stars : public EnvironObject
{
    private:
        std::vector<StarsData> mStars;
        std::vector<ImageStarsData> mImageStars;

        float mBloomSize;
        float t1, t2;
        bool mNight, mDrawStars;

        CL_GraphicContext mGC;

        // For galaxies
        CL_Image mBigGalaxy;
        float mBigGalaxyAlpha, mBigGalaxyBloom;

    public:
        Stars(int maxStars = 200);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_STARS_HPP_ */
