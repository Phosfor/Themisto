#include "World/Environ/Lightnings.hpp"
#include "Core/EnvironManager.hpp"

Lightnings::Lightnings()
    : EnvironObject(), mProbability(1000)
{
    mGC = appManager.getGraphic();
    mLightningImage = CL_Image(mGC, "media/lightnings/1.png");
}

void Lightnings::update(float windPower, float elapsed, float globalTime)
{
    //if (rand() % mProbability == 0 && !mAnimation)
    //{
        mTimer += elapsed * 1000.0f;
        mLightningImage.draw(mGC, 200, 100);
        //mAnimation = true;
    //}
}
