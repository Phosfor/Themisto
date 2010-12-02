/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Environ/Stars.hpp"

void Stars::setLimit(int limit)
{
   mStars.resize(limit, StarsData(mWindowWidth, mWindowHeight));
   mMaxObjects = limit;
}

StarsData::StarsData(int width, int height)
{
    x = rand() % width;
    y = rand() % height;

    brightness = (float)(rand() % 10 + 1)/10.0f;

    color = CL_Colorf(1.0f, 1.0f, 1.0f, brightness);
}

Stars::Stars(int maxStars)
    : EnvironObject(), mBloomSize(0.0f), t1(0), t2(0)
    , mNight(false), mDrawStars(false)
{
    srand(time(NULL));

    mGC = appManager.getGraphic();
    mMaxObjects = maxStars;

    for (int i=0; i < mMaxObjects; i++)
        mStars.push_back(StarsData(mWindowWidth, mWindowHeight));
}

void Stars::update(float windPower, float elapsed, float globalTime)
{
    if (globalTime <= 10.0f) {
        t1 = 0.0f;
    } else if (globalTime >= 13.0f) {
        t1 = 1.0f;
        mNight = true;
    } else {
        mDrawStars = true;
        t1 = ((globalTime - 10.0f) / (13.0f - 10.0f)) / 5.0f;
        mBloomSize = t1;
    }

    if (mNight)
    {
        if (globalTime <= 21.5f) {
            t2 = 0.0f;
        } else if (globalTime >= 23.8f) {
            t2 = 1.0f;
            mNight = false;
            mDrawStars = false;
        } else {
            t2 = ((globalTime - 10.0f) / (13.0f - 10.0f)) / 5.0f;
            mBloomSize = 1.0f - t2;
        }
    }

    if (!mDrawStars) return;

    for (int i=0; i < mMaxObjects; i++)
    {
        // Stars "blinking"
        if (rand() % 60 == 0) continue;

        if (mBloomSize <= mStars[i].brightness)
            mStars[i].color.set_alpha(mBloomSize);

        if (mNight)
            mBloomSize += 0.001f;
        else
            mBloomSize -= 0.001f;

        CL_Draw::point(mGC, mStars[i].x, mStars[i].y, mStars[i].color);
    }
}
