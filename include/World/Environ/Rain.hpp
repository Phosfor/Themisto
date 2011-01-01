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

#ifndef _WEATHER_RAIN_HPP_
#define _WEATHER_RAIN_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <boost/cstdlib.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"
#include "World/Environ/Object.hpp"

const float kTail = 0.025f;
const float kDropAlpha = 0.5f;

struct Data
{
    float x, y, x_speed, y_speed;
    uint16_t timeout;
    Data();
};

class Rain : public EnvironObject
{
    private:
        std::vector<Data> mData;
        CL_Colorf mDropColor;
        CL_GraphicContext mGC;
        bool mFirstTime;

        void processDrops(float windPower, uint16_t i);

    public:
        explicit Rain(uint16_t maxDrops = 150);
        void setLimit(uint16_t limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_RAIN_HPP_ */
