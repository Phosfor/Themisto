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

    // Rotate cloud surface at some random angle
    /*int angle = rand() % 45 + 1;
    imageHandle.set_angle(CL_Angle::from_degrees(angle));*/

    float k1 = (float)(rand()%10 + 5) / 10.0f;
    float k2 = (float)(rand()%5 + 3) / 10.0f;
    imageHandle.set_scale(k1, k2);

    cout << "Pos: " << x << "; " << y_offset << "\n";
    cout << "Scaled with koef: " << k1 << "; " << k2 << "\n";
}

void Cloud::update(float windPower, float elapsed)
{
    x += x_speed * elapsed;

    if (windPower > 0) {
        if (x > mWidth + 10) mRemove = true;
    } else {
        if (x < -imageHandle.get_width()) mRemove = true;
    }

    x_speed = 40.0 * windPower * elapsed;
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
