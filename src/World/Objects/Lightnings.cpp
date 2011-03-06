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

#include "World/Objects/Lightnings.hpp"
#include "World/Objects/Clouds.hpp"

Lightnings::Lightnings()
    : mProbability(1000), mAnimation(false), alpha(0)
{
    mGC = appManager().getGraphic();
    uint16_t size = resourceManager().sectionHandle("Lightnings").get_child_nodes().get_length();

    for (uint16_t i=0; i < size; ++i)
        mLightningImages.push_back(resourceManager().getImage("Lightnings", boost::lexical_cast<std::string>(i)));

    if (!environManager().getTypeEnabled(Environ_Clouds))
        mEnabled = false;
    else
        mCloudsHandle = reinterpret_cast<Clouds*>(environManager().getTypeHandle(Environ_Clouds));

    mWindowWidth = levelManager().getCamViewport().get_width();
    mWindowHeight = levelManager().getCamViewport().get_height();
}

void Lightnings::update(float elapsed)
{
    if (mEnabled && !mAnimation && rand() % mProbability == 0)
    {
        mPosition = mCloudsHandle->getCloudPos();
        if (mPosition.x != -1 && mPosition.y != -1)
        {
            mAnimation = true;
            mHandle = rand() % mLightningImages.size();
        }
    }

    if (mEnabled && mAnimation && mTimer <= 900.0f)
    {
        mTimer += elapsed * 1000.0f;
        mLightningImages[mHandle].set_alpha(0.8);
    }
    else
    {
        mTimer = 0;
        mAnimation = false;
    }
}

void Lightnings::updateVisual(float newX, float newY)
{
    if (mEnabled && mAnimation && mTimer <= 900.0f)
    {
        mLightningImages[mHandle].draw(mGC, mPosition.x, mPosition.y);
    }
}

void Lightnings::setPosition(CL_Pointf newPos)
{
}
CL_Pointf Lightnings::getPosition()
{
     return levelManager().getCamViewport().get_top_left();
}
CL_Rectf Lightnings::getRectangle()
{
    return levelManager().getCamViewport();
}

boost::shared_ptr<Object> Lightnings::ParseLightnings(CL_DomElement* element, std::string& desc)
{
    return boost::shared_ptr<Object>(new Lightnings());
}
