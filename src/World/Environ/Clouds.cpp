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

#include "World/Environ/Clouds.hpp"
#include "Core/EnvironManager.hpp"

void Clouds::setLimit(int limit)
{
   mClouds.resize(limit);
   if (limit > mMaxObjects && !mFirstTime)
      for (int i=0; i < limit-mMaxObjects; i++)
         processClouds(mGC, environManager().getWindPower(), i);

   mMaxObjects = limit;
}

void Clouds::processClouds(CL_GraphicContext &gc, float windPower, int i)
{
    mClouds[i].y_offset = rand() % (int)(mWindowHeight * 0.05);

    mClouds[i].x_speed = 0;
    int size = resourceManager().sectionHandle("Clouds").get_child_nodes().get_length();
    mClouds[i].cloudType = rand() % size;
    mClouds[i].imageHandle = resourceManager().getSprite("Clouds", boost::lexical_cast<std::string>(mClouds[i].cloudType));

    if (!mFirstTime)
    {
        if (windPower < 0)
            mClouds[i].x = mWindowWidth;
        else
            mClouds[i].x = 0 - mClouds[i].imageHandle.get_width();

        mClouds[i].timeout = rand() % 300;
    }
    else
    {
        mClouds[i].x = rand() % mWindowWidth;
        mClouds[i].timeout = 0;
    }

    float alpha = (float)(rand()%4 + 7) / 10.0f;
    float color = (float)(rand()%1 + 3) / 10.0f;
    mClouds[i].mColor = CL_Colorf(color, color, color, alpha);

    mClouds[i].speed_koef = rand() % 35 + 45;
}

Clouds::Clouds(int maxClouds)
    : EnvironObject(), mFirstTime(true)
{
    srand(time(NULL));

    mMaxObjects = maxClouds;
    mGC = appManager().getGraphic();
    mFirstTime = true;
}

void Clouds::update(float windPower, float elapsed, float globalTime)
{
    if (mFirstTime)
    {
        for (int i=0; i < mMaxObjects; i++)
        {
            mClouds.push_back(CloudData());
            processClouds(mGC, windPower, i);
        }

        mFirstTime = false;
    }

    for (int i=0; i < mMaxObjects; i++)
    {
        if (mClouds[i].timeout > 0)
        {
            mClouds[i].timeout--;
        }
        else
        {
            mClouds[i].x += mClouds[i].x_speed * elapsed;
            mClouds[i].x_speed = mClouds[i].speed_koef * windPower * elapsed;

            if (windPower > 0) {
                if (mClouds[i].x > mWindowWidth)
                    processClouds(mGC, windPower, i);
            } else {
                if (mClouds[i].x < -mClouds[i].imageHandle.get_width())
                    processClouds(mGC, windPower, i);
            }

            mClouds[i].imageHandle.set_color(mClouds[i].mColor);
            mClouds[i].imageHandle.draw(mGC, mClouds[i].x, mClouds[i].y_offset);
        }
    }
}

CL_Pointf Clouds::getCloudPos()
{
    int counter = 0;
    CL_Pointf result(-1, -1);
    while (result.x == -1)
    {
        ++counter;
        if (counter == 4) break;

        int index = rand() % mMaxObjects + 1;
        if (mClouds[index].x > mWindowWidth*0.1 &&
            mClouds[index].x < mWindowWidth - mWindowWidth*0.1)
        {
            result.x = mClouds[index].x;
            result.y = mClouds[index].y_offset + 
                mClouds[index].imageHandle.get_height() / 2.0f;
        }
        else
        {
            continue;
        }
    }

    return result;
}
