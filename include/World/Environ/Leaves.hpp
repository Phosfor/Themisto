#ifndef _WEATHER_LEAVES_HPP_
#define _WEATHER_LEAVES_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "World/Environ/Object.hpp"

struct LeafData
{
    float x, y, x_speed, y_speed, k1, k2, speed_koef;
    int leafType, timeout;

    CL_Sprite imageHandle;

    bool animation;
    float addedAngle, timer;
};

class Leaves : public EnvironObject
{
    private:
        vector<LeafData> mLeaves;
        bool mFirstTime;

        CL_GraphicContext mGC;

        void processLeaves(CL_GraphicContext &gc, float windPower, int i);

    public:
        Leaves(int maxLeaves = 5);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_LEAVES_HPP_ */
