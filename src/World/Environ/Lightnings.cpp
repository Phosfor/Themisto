/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Environ/Lightnings.hpp"
#include "Core/EnvironManager.hpp"

Lightnings::Lightnings()
    : EnvironObject(), mProbability(200), alpha(0)
{
    mGC = appManager().getGraphic();

    for (int i=0; i < 11; ++i)
    {
        mLightningImage[i] = CL_Image(mGC, cl_format("media/lightnings/%1.png", i));
    }

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
            mHandle = rand() % 11;
        }
    }

    if (mAnimation && mTimer <= 700.0f)
    {
        mTimer += elapsed * 1000.0f;
        mLightningImage[mHandle].draw(mGC, mPosition.x, mPosition.y);
    }
    else
    {
        mTimer = 0;
        mAnimation = false;
    }
}
