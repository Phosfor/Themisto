#include "World/Environ/Rain.hpp"

void Rain::setLimit(int limit)
{
   mData.resize(limit, Data());
   mMaxObjects = limit;
}

Data::Data()
    : x(rand()%ScreenResolutionX), y(0)
    , x_speed(), y_speed(0), timeout(rand()%130)
{ }

void Rain::processDrops(float windPower, int i)
{
    int left, right;
    left = right = 0;

    int x1 = (mWindowHeight / G) * windPower;

    if (windPower < 0) {
        left = 0; 
        right = mWindowWidth - x1;
    } else {
        left = -x1;
        right = mWindowWidth;
    }

    int posX = rand() % (right - left) + left;

    mData[i].x = posX;
    mData[i].y = 0;

    mData[i].x_speed = windPower*0.1;
    mData[i].y_speed = G*0.1;

    mData[i].timeout = rand() % 130;
}

Rain::Rain(int maxDrops)
    : EnvironObject(), mData(maxDrops)
{
    srand(time(NULL));
    mMaxObjects = maxDrops;

    mDropColor = CL_Colorf::black;
    mDropColor.a = kDropAlpha;

    mGC = appManager.getGraphic();
}

void Rain::update(float windPower, float elapsed, float globalTime)
{
    for (int i=0; i < mMaxObjects; i++)
    {
        if (mData[i].timeout > 0)
        {
            mData[i].timeout--;
        }
        else
        {
            mData[i].x += mData[i].x_speed * elapsed;
            mData[i].y += mData[i].y_speed * elapsed;

            mData[i].x_speed += windPower * elapsed;
            mData[i].y_speed += G * elapsed;

            CL_Draw::line(mGC,
                    mData[i].x, mData[i].y, 
                    mData[i].x - mData[i].x_speed * kTail, mData[i].y - mData[i].y_speed * kTail, 
                    mDropColor);

            if (mData[i].y > mWindowHeight) processDrops(windPower, i);
        }
    }
}
