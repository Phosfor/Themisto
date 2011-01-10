/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

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

#include "World/Environ/Birds.hpp"

void Birds::setLimit(uint16_t limit)
{
   mBirds.resize(limit);
   if (limit > mMaxObjects && !mFirstTime)
      for (uint16_t i=0; i < limit-mMaxObjects; i++)
         processBirds(mGC, mWindowWidth, mWindowHeight, i);

   mMaxObjects = limit;
}

void Birds::processBirds(CL_GraphicContext &gc, uint16_t width, uint16_t height, uint16_t i)
{
    CL_DomElement birds = resourceManager().sectionHandle("Birds").to_element();
    uint16_t numBirds = birds.get_child_nodes().get_length();

    CL_DomElement birdHandle;
    if (numBirds > 1)
        birdHandle = birds.get_child_nodes().item(rand() % numBirds + 1).to_element();
    else
        birdHandle = birds.get_first_child_element();

    std::vector<std::string> textures;
    // TODO: Chose the bird type here
    for (uint16_t j=0; j < birdHandle.get_child_nodes().get_length(); j++)
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

    mBirds[i].y = height * static_cast<float>((rand()%3 + 2)) / 10.0;
}

Birds::Birds(uint16_t maxBirds)
    : EnvironObject(), mProbability(1000), mFirstTime(true)
{
    mGC = appManager().getGraphic();
    mMaxObjects = maxBirds;

    for (uint16_t i=0; i < maxBirds; i++)
        mBirds.push_back(BirdData());
}

void Birds::update(float elapsed)
{
    if (mFirstTime)
    {
        for (uint16_t i=0; i < mMaxObjects; i++)
            processBirds(mGC, mWindowWidth, mWindowHeight, i);
        mFirstTime = false;
    }
    for (uint16_t i=0; i < mMaxObjects; i++)
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
