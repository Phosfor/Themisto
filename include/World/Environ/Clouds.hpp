#ifndef _WEATHER_CLOUDS_HPP_
#define _WEATHER_CLOUDS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <boost/foreach.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class Cloud
{
    private:
        float x, y_offset, x_speed;
        float speed_koef;
        int cloudType;
        CL_Sprite imageHandle;
        CL_Colorf mColor;

        bool mRemove;

        CL_GraphicContext mGC;
        int mWidth, mHeight;

    public:
        Cloud(float windPower);

        void update(float windPower, float elapsed);
        bool getRemove();
};

class Clouds
{
    private:
        vector<Cloud> mClouds;
        unsigned int mMaxClouds;

    public:
        Clouds(int maxClouds = 4);
        void setCloudLimit(int maxClouds);
        int getCloudLimit();
        void update(float _windPower);
};

#endif /* _WEATHER_CLOUDS_HPP_ */
