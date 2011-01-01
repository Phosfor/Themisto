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

#include "World/Environ/Stars.hpp"

void Stars::setLimit(uint16_t limit)
{
   mStars.resize(limit, StarsData(mWindowWidth, mWindowHeight));
   mMaxObjects = limit;
}

StarsData::StarsData(uint16_t width, uint16_t height)
{
    x = rand() % width;
    y = rand() % height;

    brightness = static_cast<float>((rand() % 10 + 1))/10.0f;

    color = CL_Colorf(1.0f, 1.0f, 1.0f, brightness);
}

ImageStarsData::ImageStarsData(CL_GraphicContext gc, uint16_t width, uint16_t height)
{
    x = rand() % width;
    y = rand() % height;

    brightness = static_cast<float>((rand() % 10 + 1))/10.0f;

    uint16_t size = resourceManager().sectionHandle("Stars").get_child_nodes().get_length();
    uint16_t randStar = rand() % size;
    imageHandle = resourceManager().getImage("Stars", boost::lexical_cast<std::string>(randStar));

    float scale = static_cast<float>(rand()%2 + 2)/10.0f;
    imageHandle.set_scale(scale, scale);
    imageHandle.set_alpha(brightness);
}

Stars::Stars(uint16_t maxStars)
    : EnvironObject(), mBloomSize(0.0f), t1(0), t2(0)
    , mNight(false), mDrawStars(false), mBigGalaxyBloom(0.0f)
{
    srand(time(NULL));

    mGC = appManager().getGraphic();
    mMaxObjects = maxStars;

    for (uint16_t i=0; i < mMaxObjects; i++)
        mStars.push_back(StarsData(mWindowWidth, mWindowHeight));

    mBigGalaxy = resourceManager().getImage("Nebulas", "big_galaxy");
    mBigGalaxyAlpha = 0.8f; // Max alpha value
    mBigGalaxy.set_alpha(0.0);

    for (uint16_t i=0; i < mMaxObjects; ++i)
        mImageStars.push_back(ImageStarsData(mGC, mWindowWidth, mWindowHeight));
}

void Stars::update(float windPower, float elapsed, float globalTime)
{
    if (globalTime <= 10.0f) {
        t1 = 0.0f;
    } else if (globalTime >= 14.0f) {
        t1 = 1.0f;
        mNight = true;
    } else {
        mDrawStars = true;
        t1 = ((globalTime - 10.0f) / (14.0f - 10.0f));
        mBloomSize = t1;
        mBigGalaxyBloom = t1 / 1.5f;
    }

    if (mNight)
    {
        if (globalTime <= 20.8f) {
            t2 = 0.0f;
        } else if (globalTime >= 23.8f) {
            t2 = 1.0f;
            mNight = false;
            mDrawStars = false;
        } else {
            t2 = ((globalTime - 20.8f) / (23.8f - 20.8f));
            mBloomSize = 1.0f - t2;
            mBigGalaxyBloom = 1.0f - t2;
        }
    }

    if (!mDrawStars) return;

    if (mBigGalaxyBloom <= mBigGalaxyAlpha)
        mBigGalaxy.set_alpha(mBigGalaxyBloom);

    if (mNight && mBigGalaxyBloom < mBigGalaxyAlpha)
        mBigGalaxyBloom += 0.007f;
    else if (!mNight && mBigGalaxyBloom < mBigGalaxyAlpha)
        mBigGalaxyBloom -= 0.007f;

    mBigGalaxy.draw(mGC, 0, 0);

    for (uint16_t i=0; i < mMaxObjects; i++)
    {
        // Stars "blinking"
        if (rand() % 80 == 0) continue;

        if (mBloomSize <= mStars[i].brightness)
            mStars[i].color.set_alpha(mBloomSize);

        if (mBloomSize <= mImageStars[i].brightness)
            mImageStars[i].imageHandle.set_alpha(mBloomSize);


        if (mNight)
            mBloomSize += 0.001f;
        else
            mBloomSize -= 0.001f;

        CL_Draw::point(mGC, mStars[i].x, mStars[i].y, mStars[i].color);
        mImageStars[i].imageHandle.draw(mGC, mImageStars[i].x, mImageStars[i].y);
    }
}
