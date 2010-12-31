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

#ifndef _WEATHER_LEAVES_HPP_
#define _WEATHER_LEAVES_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "World/Environ/Object.hpp"

struct LeafData
{
    float x, y, x_speed, y_speed, k1, k2, speed_koef;
    int leafType, timeout;

    CL_Sprite imageHandle;

    bool animation;
    float addedAngle, timer;
};

class Leaves : public EnvironObject
{
    private:
        std::vector<LeafData> mLeaves;
        bool mFirstTime;
        float functionValue;

        CL_GraphicContext mGC;

        void processLeaves(CL_GraphicContext &gc, float windPower, int i);

    public:
        explicit Leaves(int maxLeaves = 5);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_LEAVES_HPP_ */
