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

#include "World/Objects/Stars.hpp"

ImageStarsData::ImageStarsData(CL_GraphicContext gc, uint16_t mActualSize)
{
    int width = ScreenResolutionX;
    int height = ScreenResolutionY;

    x = rand() % width;
    y = rand() % (height - (height * mActualSize / 100));
    //y = rand() % height;

    brightness = static_cast<float>((rand() % 10 + 2))/10.0f;

    uint16_t mNumStars = resourceManager().sectionHandle("Stars").get_child_nodes().get_length();
    uint16_t randStar = rand() % mNumStars;
    imageHandle = resourceManager().getImage("Stars", boost::lexical_cast<std::string>(randStar));

    float scale = static_cast<float>(rand()%13 + 9)/100.0f;
    imageHandle.set_scale(scale, scale);
    imageHandle.set_alpha(brightness);
}

Stars::Stars(uint16_t maxStars)
    : mBloomSize(0.0f), t1(0), t2(0)
    , mNight(false), mDrawStars(false), mBigGalaxyBloom(0.0f)
{
    srand(time(NULL));

    mGC = appManager().getGraphic();
    mMaxObjects = maxStars;

    // TODO: scale nebula
    mBigGalaxy = resourceManager().getImage("Nebulas", "big_galaxy");
    mBigGalaxyAlpha = 0.8f; // Max alpha value
    mBigGalaxy.set_alpha(0.0);
    mGalaxyX = (ScreenResolutionX  - mBigGalaxy.get_width()) / 2;

}

void Stars::init()
{
    boost::shared_ptr<Foreground> temp = levelManager().getObjectByType<Foreground>("Foreground");
    if (temp->getType() != "Empty") mActualSize = temp->getActualSize();

    for (uint16_t i=0; i < mMaxObjects; ++i)
        mImageStars.push_back(ImageStarsData(mGC, mActualSize));
}

void Stars::update(float elapsed)
{
    float globalTime = worldManager().getAbsTime();
    if (globalTime <= 10.0f) {
        t1 = 0.0f;
    } else if (globalTime >= 14.0f) {
        t1 = 1.0f;
        mNight = true;
    } else {
        mDrawStars = true;
        t1 = ((globalTime - 10.0f) / (14.0f - 10.0f));
        mBloomSize = t1;
        mBigGalaxyBloom = t1 * 0.5f;
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


    if (mNight)
        mBloomSize += 0.001f;
    else
        mBloomSize -= 0.001f;
}

void Stars::updateVisual(float newX, float newY)
{
    if (!mDrawStars) return;

    mBigGalaxy.draw(mGC, mGalaxyX, 0);

    for (uint16_t i=0; i < mMaxObjects; i++)
    {
        // Stars "blinking"
        if (rand() % 70 == 0) continue;

        ImageStarsData &data2 = mImageStars[i];

        if (mBloomSize <= data2.brightness)
            data2.imageHandle.set_alpha(mBloomSize);

        data2.imageHandle.draw(mGC, data2.x, data2.y);
    }
}

boost::shared_ptr<Object> Stars::ParseStars(CL_DomElement tag)
{
    float max = 0;
    if(tag.has_attribute("maxStars"))
    {
        std::string maxStr = tag.get_attribute("maxStars").c_str();
        max = boost::lexical_cast<float>(maxStr);
    }

    float area = Pixels2Meters(ScreenResolutionX) * Pixels2Meters(ScreenResolutionY);
    return boost::shared_ptr<Object>(new Stars(max * area));
}

bool Stars::checkCollision(CL_Pointf point) { return false; }

CL_Rectf Stars::getRectangle()
{
    return CL_Rectf(0, 0, ScreenResolutionX, ScreenResolutionY);
}

CL_Pointf Stars::getPosition()
{
    return CL_Pointf(0, 0);
}

void Stars::setPosition(CL_Pointf newPos)
{
}
