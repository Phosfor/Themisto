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

#include "World/Environ/Leaves.hpp"

void Leaves::setLimit(int limit)
{
   mLeaves.resize(limit);
   if (limit > mMaxObjects && !mFirstTime)
      for (int i=0; i < limit-mMaxObjects; i++)
         processLeaves(mGC, 0, i);

   mMaxObjects = limit;
}


void Leaves::processLeaves(CL_GraphicContext &gc, float windPower, int i)
{
    mLeaves[i].timer = mLeaves[i].addedAngle = 0;
    mLeaves[i].x_speed = mLeaves[i].y_speed = 0;

    mLeaves[i].animation = false;

    // Where to create new leaf (refers to the wind blowing direction)
    if (mFirstTime)
    {
        mLeaves[i].x = rand() % mWindowWidth;
        mLeaves[i].timeout = 0;
    }
    else
    {
        if (windPower < 0)
            mLeaves[i].x = mWindowWidth;
        else
            mLeaves[i].x = 0;

        mLeaves[i].timeout = rand() % 250;
    }

    // The height where new leaf will be located
    mLeaves[i].y = mWindowHeight - mWindowHeight*((rand()%6+2)/10.0);

    // Some magic numbers for the leaf trajectory
    mLeaves[i].k1 = rand() % 30 + 20;
    mLeaves[i].k2 = (rand() % 7 + 3) / 10.0;

    // Load some random leaf surface
    int size = resourceManager().sectionHandle("Leaves").get_child_nodes().get_length();
    mLeaves[i].leafType = rand() % size;

    mLeaves[i].imageHandle = resourceManager().getSprite("Leaves", boost::lexical_cast<std::string>(mLeaves[i].leafType));

    // Rotate leaf surface at some random angle
    int angle = rand() % 360 + 1;
    mLeaves[i].imageHandle.set_angle(CL_Angle::from_degrees(angle));

    mLeaves[i].speed_koef = rand()%70 + 50;
}

Leaves::Leaves(int maxLeaves)
    : EnvironObject(), mFirstTime(true)
{
    srand(time(NULL));
    mGC = appManager().getGraphic();
    mMaxObjects = maxLeaves;

    functionValue = +1;
}

void Leaves::update(float windPower, float elapsed, float globalTime)
{
    // Init pack part of leaves
    if (mFirstTime)
    {
        for (int i=0; i < mMaxObjects; ++i)
        {
            mLeaves.push_back(LeafData());
            processLeaves(mGC, windPower, i);
        }
        mFirstTime = false;
    }

    for (int i=0; i < mMaxObjects; i++)
    {
        if (mLeaves[i].timeout > 0)
        {
            mLeaves[i].timeout--;
        }
        else
        {
            mLeaves[i].x += mLeaves[i].x_speed * elapsed;
            mLeaves[i].y += mLeaves[i].y_speed * elapsed;

            float y = mLeaves[i].k1 * sinf(mLeaves[i].k2 * Deg2Rad(mLeaves[i].x)) + mLeaves[i].y;

            float tempValue = mLeaves[i].k1 * mLeaves[i].k2 * cosf(mLeaves[i].k2 * Deg2Rad(mLeaves[i].x));

            if (tempValue * functionValue < 0)
            {
                mLeaves[i].animation = true;
            }
            functionValue = tempValue;

            if (mLeaves[i].animation)
            {
                mLeaves[i].timer += elapsed * 1000.0;
                if (mLeaves[i].timer >= 100)
                {
                    int angle = rand() % 15 + 5;
                    mLeaves[i].addedAngle += angle;

                    mLeaves[i].imageHandle.rotate_pitch(CL_Angle::from_degrees(angle));

                    mLeaves[i].timer = 0;

                    if (mLeaves[i].addedAngle >= 180)
                    {
                        mLeaves[i].animation = false;
                        mLeaves[i].addedAngle = 0;
                    }
                }
            }

            if (windPower > 0) {
                if (mLeaves[i].x > mWindowWidth + 10 || mLeaves[i].y > mWindowHeight) 
                    processLeaves(mGC, windPower, i);
            } else {
                if (mLeaves[i].x < -mLeaves[i].imageHandle.get_width() || mLeaves[i].y > mWindowHeight)
                    processLeaves(mGC, windPower, i);
            }

            mLeaves[i].x_speed = mLeaves[i].speed_koef * windPower * elapsed;
            mLeaves[i].y_speed = Gravity * elapsed;

            mLeaves[i].imageHandle.draw(mGC, mLeaves[i].x, y);
        }
    }
}
