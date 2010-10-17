#include "World/Environ/Clouds.hpp"

Cloud::Cloud(float windPower)
{
    mGC = appManager.getGraphic();
    mWidth = mGC.get_width();
    mHeight = mGC.get_height();

    if (windPower < 0)
        x = mWidth + rand() % 500;
    else
        x = 0 - rand() % 500;

    y_offset = rand() % (int)(mHeight * 0.05);

    x_speed = 0.1;
    cloudType = rand()%8;
    imageHandle = CL_Sprite(mGC, cl_format("media/clouds/%1.png", cloudType));

    /*float k1 = (float)(rand()%5 + 3) / 10.0f;
    float k2 = (float)(rand()%5 + 3) / 10.0f;
    imageHandle.set_scale(k1, k2);*/

    float alpha = (float)(rand()%5 + 6) / 10.0f;
    float color = (float)(rand()%5 + 6) / 10.0f;
    mColor = CL_Colorf(color, color, color, alpha);

    speed_koef = rand() % 35 + 45;
}

void Cloud::update(float windPower, float elapsed)
{
    x += x_speed * elapsed;

    if (windPower > 0) {
        if (x > mWidth + 10) mRemove = true;
    } else {
        if (x < -imageHandle.get_width()) mRemove = true;
    }

    x_speed = speed_koef * windPower * elapsed;

    imageHandle.set_color(mColor);
    imageHandle.draw(mGC, x, y_offset);
}

bool Cloud::getRemove()
{
    return mRemove;
}

Clouds::Clouds(int maxClouds)
{
    srand(time(NULL));
    mMaxClouds = maxClouds;
}

void Clouds::setCloudLimit(int maxClouds)
{
    mMaxClouds = maxClouds;
}

int Clouds::getCloudLimit()
{
    return mMaxClouds;
}

void Clouds::update(float _windPower)
{
    float elapsed = appManager.getElapsed() / 1000.0f;

    if (mClouds.size() < mMaxClouds)
    {
        if (rand()%80 == 0) mClouds.push_back(Cloud(_windPower));
    }

    for (unsigned int i=0; i < mClouds.size(); i++)
    {
        mClouds[i].update(_windPower, elapsed);
        if (mClouds[i].getRemove()) mClouds.erase(mClouds.begin() + i);
    }
}
