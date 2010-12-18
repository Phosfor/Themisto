/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Environ/Birds.hpp"

void Birds::setLimit(int limit)
{
   mBirds.resize(limit);
   if (limit > mMaxObjects && !mFirstTime)
      for (int i=0; i < limit-mMaxObjects; i++)
         processBirds(mGC, mWindowWidth, mWindowHeight, i);

   mMaxObjects = limit;
}

void Birds::processBirds(CL_GraphicContext &gc, int width, int height, int i)
{
    // TODO: Chose the bird type here
    for (int j=1; j <= 10; j++)
    {
        // Load all animation frames
        mBirds[i].mImageDesc.add_frame(CL_ImageProviderFactory::load(cl_format("media/bird/%1.png", j)));
    }

    mBirds[i].mBirdImage = CL_Sprite(gc, mBirds[i].mImageDesc);
    mBirds[i].mBirdImage.set_scale(0.5, 0.5);
    mBirds[i].mBirdImage.set_linear_filter(true);

    mBirds[i].x_speed = rand() % 30 + 150;
    mBirds[i].timeout = rand() % 500;

    if (rand()%2 == 0) {
        mBirds[i].x = 0 - mBirds[i].mBirdImage.get_width();
        mBirds[i].side = 1; // The speed should be less than zero (to make flying left)
        mBirds[i].mBirdImage.set_angle_yaw(CL_Angle::from_degrees(180));
    } else {
        mBirds[i].x = width;
        mBirds[i].side = -1;
        mBirds[i].mBirdImage.set_angle_yaw(CL_Angle::from_degrees(0));
    }

    mBirds[i].y = height * (float)(rand()%3 + 2) / 10.0;
}

Birds::Birds(int maxBirds)
    : EnvironObject(), mProbability(1000), mFirstTime(true)
{
    mGC = appManager().getGraphic();
    mMaxObjects = maxBirds;

    for (int i=0; i < maxBirds; i++)
        mBirds.push_back(BirdData());
}

void Birds::update(float windPower, float elapsed, float globalTime)
{
    if (mFirstTime)
    {
        for (int i=0; i < mMaxObjects; i++)
            processBirds(mGC, mWindowWidth, mWindowHeight, i);
        mFirstTime = false;
    }
    for (int i=0; i < mMaxObjects; i++)
    {
        if (mBirds[i].timeout > 0)
        {
            mBirds[i].timeout--;
        }
        else
        {
            mBirds[i].x += mBirds[i].side * mBirds[i].x_speed * elapsed;
            mBirds[i].mBirdImage.update();
            mBirds[i].mBirdImage.draw(mGC, mBirds[i].x, mBirds[i].y);

            if (mBirds[i].side == 1 && mBirds[i].x >= mWindowWidth) 
                processBirds(mGC, mWindowWidth, mWindowHeight, i);
            else if (mBirds[i].side == -1 && mBirds[i].x <= 0 - mBirds[i].mBirdImage.get_width())
                processBirds(mGC, mWindowWidth, mWindowHeight, i);
        }
    }
}
