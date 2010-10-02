#include "World/Weather/Rain.hpp"

// Drop structure 
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
}

// Rain manager
void Rain::processDrops(float _windPower)
{
    // Maybe enough drops?
    /*int n = min(kDropPerFrame, mMaxDrops - mDrops.size());
    for (int i=0; i < n; i++)*/
    if (mDrops.size() > mMaxDrops) return;

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
    mDrops.push_back(drop(posX, _windPower));
}

Rain::Rain(int maxDrops):
    mMaxDrops(maxDrops)
{ 
    srand ( time(NULL) );
}

void Rain::update(float _windPower)
{
    //mWindPower = - ((rand()%9)/10 + 6.0) * 50;
    unsigned int counter = 0;
    BOOST_FOREACH(drop &d, mDrops)
    {
        /*TODO: Remove boost foreach and remove drops in another cycle */
        d.update(appManager.getElapsed(), _windPower);
        if (d.getY() > appManager.getWindow().get_geometry().get_height()-100)
            mDrops.erase(mDrops.begin()+counter);
        counter++;

    }
    processDrops(_windPower);
}

void Rain::setDropLimit(int maxDrops)
{
    mMaxDrops = maxDrops;
}
