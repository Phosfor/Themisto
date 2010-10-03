#ifndef _WEATHER_RAIN_HPP_
#define _WEATHER_RAIN_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <boost/foreach.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

#define kTail 0.025
#define kDropAlpha 0.5

class Rain
{
    private:
        /*float *x, *y;
        float *x_speed, *y_speed;*/

        vector<int> x, y, x_speed, y_speed, timeout;

        //int *timeout;

        unsigned int mMaxDrops;

        float mWidth, mHeight;

        CL_Colorf mDropColor;
        CL_GraphicContext mGC;

        void processDrops(float _windPower, int i);

    public:
        Rain(int maxDrops = 150);
        void setDropLimit(int maxDrops);
        void update(float _windPower);
};

#endif /* _WEATHER_RAIN_HPP_ */
