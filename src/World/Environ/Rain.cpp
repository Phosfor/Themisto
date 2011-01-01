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

#include "World/Environ/Rain.hpp"

void Rain::setLimit(uint16_t limit)
{
   mData.resize(limit, Data());
   mMaxObjects = limit;
}

Data::Data()
    : x(rand()%ScreenResolutionX), y(0), timeout(rand()%130)
{ }

void Rain::processDrops(float windPower, Data &current, bool firstTime)
{
    uint16_t left, right;
    left = right = 0;

    uint16_t x1 = mKoef1 * windPower;

    if (windPower < 0) {
        left = 0; 
        right = mWindowWidth - x1;
    } else {
        left = -x1;
        right = mWindowWidth;
    }

    uint16_t posX = rand() % (right - left) + left;
    current.x = posX;

    current.x_speed = windPower;
    current.y_speed = Gravity;

    if (firstTime)
    {
        current.timeout = 0;
        current.y = rand() % mWindowHeight;
    }
    else
    {
        current.timeout = rand() % 130;
        current.y = 0;
    }
}

Rain::Rain(uint16_t maxDrops)
    : EnvironObject(), mData(maxDrops)
{
    srand(time(NULL));
    mMaxObjects = maxDrops;

    mDropColor = CL_Colorf::black;
    mDropColor.a = kDropAlpha;

    mGC = appManager().getGraphic();

    mKoef1 = mWindowHeight / Gravity;
    float windPower = environManager().getWindPower();
    for (uint16_t i=0; i < mMaxObjects; ++i)
    {
        mData.push_back(Data());
        processDrops(windPower, mData[i], true);
    }
}

void Rain::update(float windPower, float elapsed, float globalTime)
{
    float newXSpeed = windPower * elapsed;
    float newYSpeed = Gravity * elapsed;
    for (uint16_t i=0; i < mMaxObjects; i++)
    {
        // Cache it, yeah
        Data &current = mData[i];
        if (current.timeout > 0)
        {
            current.timeout--;
        }
        else
        {
            if (current.y > mWindowHeight || current.x > mWindowWidth || current.x < 0)
                processDrops(windPower, current, i);

            current.x += current.x_speed * elapsed;
            current.y += current.y_speed * elapsed;

            current.x_speed += newXSpeed;
            current.y_speed += newYSpeed;

            CL_Draw::line(mGC, current.x, current.y,
                    current.x - current.x_speed * kTail, current.y - current.y_speed * kTail,
                    mDropColor);
        }
    }
}
