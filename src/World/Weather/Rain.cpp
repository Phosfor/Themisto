#include "World/Weather/Rain.hpp"

void Rain::processDrops(float _windPower, int i)
{
    int left, right;
    left = right = 0;

    // Find the line(+offset) where drops fails
    int x1 = (mHeight / G) * _windPower;

    if (_windPower < 0)
    {
        left = 0; 
        right = mWidth - x1;
    }
    else
    {
        left = -x1;
        right = mWidth;
    }

    int posX = rand() % (right - left) + left;

    x[i] = posX;
    y[i] = 0;

    x_speed[i] = 0;
    y_speed[i] = 0;

    //x_speed[i] = _windPower*40*0.1;
    //y_speed[i] = G*0.1;
}

Rain::Rain(int maxDrops):
    mMaxDrops(maxDrops)
{
    srand(time(NULL));

    x = new float[maxDrops]; y = new float[maxDrops];
    x_speed = new float[maxDrops]; y_speed = new float[maxDrops];
    timeout = new int[maxDrops];

    memset(x, 0, sizeof(x[0]) * maxDrops);
    memset(y, 0, sizeof(y[0]) * maxDrops);
    memset(x_speed, 0, sizeof(x_speed[0]) * maxDrops);
    memset(y_speed, 0, sizeof(y_speed[0]) * maxDrops);
    memset(timeout, 0, sizeof(timeout[0]) * maxDrops);

    mGC = appManager.getGraphic();
    mWidth = appManager.getWindow().get_geometry().get_width();
    mHeight = appManager.getWindow().get_geometry().get_height();
    mDropColor = CL_Colorf::black;
    mDropColor.a = kDropAlpha;

    for (int i=0; i < maxDrops; i++)
    {
        timeout[i] = rand() % 100;
    }
}

void Rain::update(float _windPower)
{
    float elapsed = appManager.getElapsed()/1000;

    for (unsigned int i=0; i < mMaxDrops; i++)
    {
        if (timeout[i] > 0)
        {
            timeout[i]--;
        }
        else
        {
            x_speed[i] += _windPower * elapsed;
            x[i] += x_speed[i] * elapsed;

            y_speed[i] += G * elapsed;
            y[i] += y_speed[i] * elapsed;

            CL_Draw::line(mGC, x[i], y[i], x[i] - x_speed[i] * kTail, y[i] - y_speed[i] * kTail, mDropColor);

            if (y[i] > mHeight - 100) processDrops(_windPower, i);
        }
    }
}

void Rain::setDropLimit(int maxDrops)
{
    mMaxDrops = maxDrops;
}
