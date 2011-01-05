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
#include <boost/cstdlib.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "World/Environ/Object.hpp"
#include "Core/LevelManager.hpp"

struct ImageStarsData
{
    uint16_t x, y;
    float brightness;
    CL_Image imageHandle;

    ImageStarsData(CL_GraphicContext gc, uint16_t width, uint16_t height);
};

class Stars : public EnvironObject
{
    private:
        std::vector<ImageStarsData> mImageStars;

        float mBloomSize;
        float t1, t2;
        bool mNight, mDrawStars;

        CL_GraphicContext mGC;

        // For galaxies
        CL_Image mBigGalaxy;
        int mGalaxyX;
        float mBigGalaxyAlpha, mBigGalaxyBloom;

    public:
        explicit Stars(uint16_t maxStars = 200);
        void setLimit(uint16_t limit);
        void update(float windPower, float elapsed, float globalTime);

        // Remove this later
        static int mCounter;
};

#endif /* _WEATHER_STARS_HPP_ */
