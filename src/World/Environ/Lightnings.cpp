#include "World/Environ/Lightnings.hpp"
#include "Core/EnvironManager.hpp"

Lightnings::Lightnings()
    : EnvironObject(), mProbability(500)
{
    mGC = appManager.getGraphic();
    mLightningImage = CL_Image(mGC, "media/lightnings/1.png");

    if (!environManager.getTypeEnabled(Environ_Clouds)) 
        mEnabled = false;
    else
        mCloudsHandle = dynamic_cast<Clouds*>(environManager.getTypeHandle(Environ_Clouds));
}

void Lightnings::update(float windPower, float elapsed, float globalTime)
{
    if (mEnabled && !mAnimation && rand() % mProbability == 0)
    {
        mPosition = mCloudsHandle->getCloudPos();
        if (mPosition.x != -1 && mPosition.y != -1) mAnimation = true;
    }

    if (mAnimation && mTimer <= 500.0f)
    {
        mTimer += elapsed * 1000.0f;
        mLightningImage.draw(mGC, mPosition.x, mPosition.y);
    }
    else
    {
        mTimer = 0;
        mAnimation = false;
    }
}
