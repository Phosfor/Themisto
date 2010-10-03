#include "World/Weather/Rain.hpp"

void Rain::processDrops(float _windPower, int i)
{
    int left, right;
    left = right = 0;

    // Find the line(+offset) where drops fails
    int x1 = ( mHeight / G ) * _windPower;

    if (_windPower < 0) {
        left = 0; 
        right = mWidth - x1;
    } else {
        left = -x1;
        right = mWidth;
    }

    int posX = rand() % (right - left) + left;

    x[i] = posX;
    y[i] = 0;

    x_speed[i] = 0;
    y_speed[i] = 0;

    x_speed[i] = _windPower*0.1;
    y_speed[i] = G*0.1;
}

Rain::Rain(int maxDrops):
    mMaxDrops(maxDrops)
{
    srand(time(NULL));

    x = vector<int>(maxDrops);
    y = vector<int>(maxDrops);
    x_speed = vector<int>(maxDrops);
    y_speed = vector<int>(maxDrops);
    timeout = vector<int>(maxDrops);

    fill(x.begin(), x.end(), 0);
    fill(y.begin(), y.end(), 0);
    fill(x_speed.begin(), x_speed.end(), 0);
    fill(y_speed.begin(), y_speed.end(), 0);
    fill(timeout.begin(), timeout.end(), 0);

    // For preventing possible reallocation
    x.reserve(maxDrops*1.5);
    y.reserve(maxDrops*1.5);
    x_speed.reserve(maxDrops*1.5);
    y_speed.reserve(maxDrops*1.5);

    mGC = appManager.getGraphic();
    mWidth = appManager.getWindow().get_geometry().get_width();
    mHeight = appManager.getWindow().get_geometry().get_height();
    mDropColor = CL_Colorf::black;
    mDropColor.a = kDropAlpha;

    for (int i=0; i < maxDrops; i++)
    {
        timeout[i] = rand() % 130;
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
            x[i] += x_speed[i] * elapsed;
            y[i] += y_speed[i] * elapsed;

            x_speed[i] += _windPower * elapsed;
            y_speed[i] += G * elapsed;

            CL_Draw::line(mGC,
                    x[i], y[i], 
                    x[i] - x_speed[i] * kTail, y[i] - y_speed[i] * kTail, 
                    mDropColor);

            if (y[i] > mHeight) processDrops(_windPower, i);
        }
    }
}

void Rain::setDropLimit(int maxDrops)
{
    x.resize(maxDrops, 0);
    y.resize(maxDrops, 0);
    x_speed.resize(maxDrops, 0);
    y_speed.resize(maxDrops, 0);
    timeout.resize(maxDrops, 0);

    if (maxDrops > mMaxDrops)
    {
        for (unsigned int i=0; i < maxDrops - mMaxDrops; i++)
        {
            timeout[mMaxDrops+i] = rand() % 100;
        }
    }

    mMaxDrops = maxDrops;
}
