#include "World/Environ/Lightnings.hpp"
#include "Core/EnvironManager.hpp"

Lightnings::Lightnings()
    : EnvironObject(), mProbability(1000)
{
    mGC = appManager.getGraphic();
    mLightningImage = CL_Image(mGC, "media/lightnings/1.png");

    if (!environManager.getTypeEnabled(Environ_Clouds)) 
    {
        mEnabled = false;
    }
}

void Lightnings::update(float windPower, float elapsed, float globalTime)
{
    if (!mAnimation && rand() % mProbability == 0)
    {
        mAnimation = true;
    }
    if (mAnimation && mTimer < 100)
    {
        mTimer += elapsed * 1000.0f;
        mLightningImage.draw(mGC, 200, 100);
    }
    else
    {
        mTimer = 0;
        mAnimation = false;
    }
}
