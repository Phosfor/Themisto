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
        explicit Stars(int maxStars = 200);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_STARS_HPP_ */
