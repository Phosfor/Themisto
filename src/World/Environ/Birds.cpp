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
    CL_DomElement birds = resourceManager().sectionHandle("Birds").to_element();
    int numBirds = birds.get_child_nodes().get_length();

    CL_DomElement birdHandle;
    if (numBirds > 1)
        birdHandle = birds.get_child_nodes().item(rand() % numBirds + 1).to_element();
    else
        birdHandle = birds.get_first_child_element();

    std::vector<std::string> textures;
    // TODO: Chose the bird type here
    for (int j=0; j < birdHandle.get_child_nodes().get_length(); j++)
        textures.push_back(cl_format(boost::lexical_cast<std::string>(j)));

    mBirds[i].mBirdImage = 
        resourceManager().getSprite(cl_format("Birds/%1", birdHandle.get_attribute("name")).c_str(), textures);
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
