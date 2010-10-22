#ifndef _WEATHER_CLOUDS_HPP_
#define _WEATHER_CLOUDS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <boost/foreach.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "World/Environ/EnvironObject.hpp"

struct CloudData
{
        float x, y_offset, x_speed, speed_koef;
        int cloudType, timeout;
        CL_Sprite imageHandle;
        CL_Colorf mColor;
};

class Clouds : public EnvironObject
{
    private:
        std::vector<CloudData> mClouds;
        CL_GraphicContext mGC;
        bool mFirstTime;

        void processClouds(CL_GraphicContext &gc, float windPower, int i, bool firstTime = false);

    public:
        Clouds(int maxClouds = 4);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_CLOUDS_HPP_ */
