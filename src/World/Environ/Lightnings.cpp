/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Environ/Lightnings.hpp"
#include "Core/EnvironManager.hpp"

Lightnings::Lightnings()
    : EnvironObject(), mProbability(1000), mAnimation(false), alpha(0)
{
    mGC = appManager().getGraphic();
    int size = resourceManager().sectionHandle("Lightnings").get_child_nodes().get_length();

    for (int i=0; i < size; ++i)
        mLightningImages.push_back(resourceManager().getImage("Lightnings", boost::lexical_cast<std::string>(i)));

    if (!environManager().getTypeEnabled(Environ_Clouds))
        mEnabled = false;
    else
        mCloudsHandle = dynamic_cast<Clouds*>(environManager().getTypeHandle(Environ_Clouds));
}

void Lightnings::update(float windPower, float elapsed, float globalTime)
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
