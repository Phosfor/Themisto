#include "World/Weather/Rain.hpp"

/*// Drop structure 
drop::drop(float _x, float _wind):
    x(_x), y(0), x_new(0), y_new(0), 
    speed_x(_wind*0.1), speed_y(G*0.1)
{ }

float drop::getY() { return y; }

void drop::update(float elapsed, float windPower)
{
    // Get elapsed time in seconds
    float elapsedSec = elapsed/1000;

    // Calculate new drop position
    y += speed_y * elapsedSec;
    speed_y += G * elapsedSec;

    // Update the speed
    x += speed_x * elapsedSec;
    speed_x += windPower * elapsedSec;

    // Find the tail position
    x_new = speed_x * kTail;
    y_new = speed_y * kTail;

    // Some drop color configuration
    CL_Colorf dropColor = CL_Colorf::black;
    dropColor.a = kDropAlpha;

    // Render drop as a line from head to the tail point
    CL_Draw::line(appManager.getGraphic(), x, y, x-x_new, y-y_new, dropColor);
}*/

// Rain manager
void Rain::processDrops(float _windPower, int i)
{
    // Maybe enough drops?
    /*int n = min(kDropPerFrame, mMaxDrops - mDrops.size());
    for (int i=0; i < n; i++)*/
    /*if (mDrops.size() > mMaxDrops) return;

    CL_Rect geom = appManager.getWindow().get_geometry();
    int left, right;
    left = right = 0;

    // Find the line(+offset) where drops fails
    int x1 = geom.get_height() / G * _windPower;
    if (_windPower < 0)
    {
        left = 0; 
        right = geom.get_width() - x1;
    }
    else
    {
        left = -x1;
        right = geom.get_width();
    }

    // Random(horizontal) position for the drop
    int posX = rand() % (right - left) + left;
    mDrops.push_back(drop(posX, _windPower));*/

    //if (mCounter > mMaxDrops) return;
    int left, right;
    left = right = 0;

    // Find the line(+offset) where drops fails
    int x1 = mHeight / G * _windPower;
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

    x_speed[i] = _windPower*0.1;
    y_speed[i] = G*0.1;
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
    //mWindPower = - ((rand()%9)/10 + 6.0) * 50;
    /*unsigned int counter = 0;
    BOOST_FOREACH(drop &d, mDrops)
    {
        [>TODO: Remove boost foreach and remove drops in another cycle <]
        d.update(appManager.getElapsed(), _windPower);
        if (d.getY() > appManager.getWindow().get_geometry().get_height()-100)
            mDrops.erase(mDrops.begin()+counter);
        counter++;

    }*/

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
            x_speed[i] += _windPower * elapsed;

            y[i] += y_speed[i] * elapsed;
            y_speed[i] += G * elapsed;

            CL_Draw::line(mGC, x[i], y[i], x[i] - x_speed[i] * kTail, y[i] - y_speed[i] * kTail, mDropColor);

            if (y[i] > mHeight - 100) processDrops(_windPower, i);
        }
    }

    //processDrops(_windPower);
}

void Rain::setDropLimit(int maxDrops)
{
    mMaxDrops = maxDrops;
}
