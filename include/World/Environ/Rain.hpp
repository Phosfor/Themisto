#ifndef _WEATHER_RAIN_HPP_
#define _WEATHER_RAIN_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "World/Environ/EnvironObject.hpp"

#define kTail 0.025
#define kDropAlpha 0.5

struct Data
{
    float x, y, x_speed, y_speed;
    int timeout;
    Data();
};

class Rain : public EnvironObject
{
    private:
        std::vector<Data> mData;
        CL_Colorf mDropColor;
        CL_GraphicContext mGC;

        void processDrops(float windPower, int i);

    public:
        Rain(int maxDrops = 150);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_RAIN_HPP_ */
