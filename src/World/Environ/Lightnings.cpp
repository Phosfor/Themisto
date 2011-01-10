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

#include "World/Environ/Lightnings.hpp"
#include "Core/EnvironManager.hpp"
#include "World/Environ/Clouds.hpp"

Lightnings::Lightnings()
    : EnvironObject(), mProbability(1000), mAnimation(false), alpha(0)
{
    mGC = appManager().getGraphic();
    uint16_t size = resourceManager().sectionHandle("Lightnings").get_child_nodes().get_length();

    for (uint16_t i=0; i < size; ++i)
        mLightningImages.push_back(resourceManager().getImage("Lightnings", boost::lexical_cast<std::string>(i)));

    if (!environManager().getTypeEnabled(Environ_Clouds))
        mEnabled = false;
    else
        mCloudsHandle = reinterpret_cast<Clouds*>(environManager().getTypeHandle(Environ_Clouds));
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
        mLightningImages[mHandle].draw(mGC, mPosition.x, mPosition.y);
    }
    else
    {
        mTimer = 0;
        mAnimation = false;
    }
}
