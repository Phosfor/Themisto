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
        explicit Clouds(int maxClouds = 4);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);

        // For the lightning
        CL_Pointf getCloudPos();
};

#endif /* _WEATHER_CLOUDS_HPP_ */
