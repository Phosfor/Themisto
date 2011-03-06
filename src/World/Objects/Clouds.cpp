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

#include "World/Objects/Clouds.hpp"
#include "World/Objects/Foreground.hpp"

void Clouds::setLimit(uint16_t limit)
{
   mClouds.resize(limit);

    if (limit > mMaxObjects)
      for (uint16_t i=0; i < limit - mMaxObjects; i++)
          processClouds(mGC, environManager().getWindPower(), mClouds[mMaxObjects + i], false);

   mMaxObjects = limit;
}

void Clouds::init()
{
    float windPower = environManager().getWindPower();
    for (uint16_t i=0; i < mMaxObjects; i++)
    {
        mClouds.push_back(CloudData());
        processClouds(mGC, windPower, mClouds[i], true);
    }
}

void Clouds::processClouds(CL_GraphicContext &gc, float windPower, CloudData &current, bool firstTime)
{
    current.x_speed = 0;
    uint16_t size = resourceManager().sectionHandle("Clouds").get_child_nodes().get_length();
    current.cloudType = rand() % size;
    current.imageHandle = resourceManager().getSprite("Clouds", boost::lexical_cast<std::string>(current.cloudType));

    if (!firstTime)
    {
        if (windPower < 0)
            current.x = mWindowWidth + current.imageHandle.get_width();
        else
            current.x = 0 - current.imageHandle.get_width();

        current.timeout = rand() % 350;
    }
    else
    {
        current.x = rand() % mWindowWidth;
        current.timeout = 0;

        boost::shared_ptr<Foreground> temp = levelManager().getObjectByType<Foreground>("Foreground");
        if (temp->getType() != "Empty") mActualSize = temp->getActualSize();
        current.y_offset = rand() %  mWindowHeight - (mWindowHeight * mActualSize / 100);
    }

    //float alpha = static_cast<float>((rand()%4 + 7)) / 10.0f;
    //float color = static_cast<float>((rand()%1 + 3)) / 10.0f;
    //current.mColor = CL_Colorf(color, color, color, alpha);
    //current.mColor = CL_Colorf(color, color, color, 1.2f);
    current.mColor = CL_Colorf(42/255.0f, 72/255.0f, 85/255.0f, 1.1f);

    current.speed_koef = rand() % 15 + 25;
}

Clouds::Clouds(uint16_t maxClouds)
    : mFirstTime(true)
{
    srand(time(NULL));

    mMaxObjects = maxClouds;
    mGC = appManager().getGraphic();
    mWindowWidth = ScreenResolutionY;
    mWindowHeight = ScreenResolutionX;
    mActualSize = 0;
}

// This function is required by Lightnings (make lightning only from position where
// Cloud is positioned at current time
CL_Pointf Clouds::getCloudPos()
{
    uint16_t counter = 0;
    CL_Pointf result(-1, -1);
    while (result.x == -1)
    {
        ++counter;
        if (counter == 4) break;

        uint16_t index = rand() % (int)mMaxObjects + 1;
        CloudData &current = mClouds[index];
        if (current.x > mWindowWidth*0.1 &&
            current.x < mWindowWidth - mWindowWidth*0.1)
        {
            result.x = current.x;
            result.y = current.y_offset +
                current.imageHandle.get_height() / 2.0f;
        }
        else
        {
            continue;
        }
    }

    return result;
}

void Clouds::setPosition(CL_Pointf newPos)
{
}
CL_Pointf Clouds::getPosition()
{
     return levelManager().getCamViewport().get_top_left();
}
CL_Rectf Clouds::getRectangle()
{
    return levelManager().getCamViewport();
}

void Clouds::update(float elapsed)
{
}

void Clouds::updateVisual(float newX, float newY)
{
    float windPower = environManager().getWindPower();
    float elapsed = appManager().getElapsed() / 1000.0f;
    float newSpeed = windPower * elapsed * 0.5;

    for (uint16_t i=0; i < mMaxObjects; i++)
    {
        CloudData &current = mClouds[i];
        if (current.timeout > 0)
        {
            current.timeout--;
        }
        else
        {
            if (windPower > 0) {
                if (current.x > mWindowWidth)
                    processClouds(mGC, windPower, current);
            } else {
                if (current.x < -current.imageHandle.get_width())
                    processClouds(mGC, windPower, current);
            }

            current.x += current.x_speed * elapsed;
            current.x_speed = current.speed_koef * newSpeed;

            current.imageHandle.set_color(current.mColor);
            current.imageHandle.draw(mGC, current.x, current.y_offset);
        }
    }
}


boost::shared_ptr<Object> Clouds::ParseClouds(CL_DomElement* cloudsElement, std::string& desc)
{
    float maxClouds = 0;
    if(cloudsElement->has_attribute("maxClouds"))
    {
        std::string maxCloudsStr = cloudsElement->get_attribute("maxClouds").c_str();
        maxClouds = boost::lexical_cast<float>(maxCloudsStr);
    }
    return boost::shared_ptr<Object>(new Clouds(maxClouds));
}
