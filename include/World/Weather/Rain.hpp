#ifndef _WEATHER_RAIN_HPP_
#define _WEATHER_RAIN_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <boost/foreach.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

#define kTail 0.025
#define kDropAlpha 0.5
#define kDropPerFrame 2
#define min(a, b) ((a) < (b) ? (a) : (b))

/*struct drop
{
    private:
        float x, y;
        float x_new, y_new;
        float speed_x, speed_y;

    public:
        drop(float _x = 0, float _wind = 0);

        void update(float elapsed, float windPower);
        float getY();
};*/

class Rain
{
    private:
        //vector<drop> mDrops;

        float *x, *y;
        float *x_speed, *y_speed;

        int *timeout;

        unsigned int mMaxDrops;
        int mWidth, mHeight;

        CL_Colorf mDropColor;
        CL_GraphicContext mGC;

        void processDrops(float _windPower, int i);

    public:
        Rain(int maxDrops = 150);
        void setDropLimit(int maxDrops = 150);
        void update(float _windPower);
};

#endif /* _WEATHER_RAIN_HPP_ */
