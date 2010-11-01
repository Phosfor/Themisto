#ifndef _WEATHER_STARS_HPP_
#define _WEATHER_STARS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "World/Environ/Object.hpp"

struct StarsData
{
    float x, y, size, brightness;
    CL_Colorf color;

    StarsData(int width, int height);
};

class Stars : public EnvironObject
{
    private:
        std::vector<StarsData> mStars;

        float mBloomSize;
        float t1, t2;
        bool mNight, mDrawStars;

        CL_GraphicContext mGC;

    public:
        Stars(int maxStars = 200);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_STARS_HPP_ */
