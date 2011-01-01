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
    : x(rand()%ScreenResolutionX), y(0)
    , x_speed(0), y_speed(0), timeout(rand()%130)
{ }

void Rain::processDrops(float windPower, uint16_t i)
{
    uint16_t left, right;
    left = right = 0;

    uint16_t x1 = (mWindowHeight / Gravity) * windPower;

    if (windPower < 0) {
        left = 0; 
        right = mWindowWidth - x1;
    } else {
        left = -x1;
        right = mWindowWidth;
    }

    uint16_t posX = rand() % (right - left) + left;

    mData[i].x = posX;

    mData[i].x_speed = windPower*0.1;
    mData[i].y_speed = Gravity*0.1;

    if (mFirstTime)
    {
        mData[i].timeout = 0;
        mData[i].y = rand() % mWindowHeight;
    }
    else
    {
        mData[i].timeout = rand() % 130;
        mData[i].y = 0;
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
    mFirstTime = true;
}

void Rain::update(float windPower, float elapsed, float globalTime)
{
    if (mFirstTime)
    {
        for (uint16_t i=0; i < mMaxObjects; ++i)
        {
            mData.push_back(Data());
            processDrops(windPower, i);
        }
        mFirstTime = false;
    }

    for (uint16_t i=0; i < mMaxObjects; i++)
    {
        if (mData[i].timeout > 0)
        {
            mData[i].timeout--;
        }
        else
        {
            mData[i].x += mData[i].x_speed * elapsed;
            mData[i].y += mData[i].y_speed * elapsed;

            mData[i].x_speed += windPower * elapsed;
            mData[i].y_speed += Gravity * elapsed;

            CL_Draw::line(mGC,
                    mData[i].x, mData[i].y,
                    mData[i].x - mData[i].x_speed * kTail, mData[i].y - mData[i].y_speed * kTail,
                    mDropColor);

            if (mData[i].y > mWindowHeight) processDrops(windPower, i);
        }
    }
}
