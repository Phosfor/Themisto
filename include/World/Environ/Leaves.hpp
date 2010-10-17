#ifndef _WEATHER_LEAVES_HPP_
#define _WEATHER_LEAVES_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class Leaf
{
    private:
        float x, y_offset, x_speed, y_speed, k1, k2;
        float speed_koef;
        int leafType;
        bool remove;
        int width, height;
        CL_Sprite imageHandle;
        CL_GraphicContext mGC;

        bool animation;
        float addedAngle;
        float timer;

    public:
        Leaf(float windPower);

        void update(float windPower, float elapsed);
        bool getRemove();
};

class Leaves
{
    private:
        vector<Leaf> mLeaves;
        unsigned int mMaxLeaves;

    public:
        Leaves(int maxLeaves = 5);
        void setLeafLimit(int maxLeaves);
        int getLeafLimit();
        void update(float _windPower);
};

#endif /* _WEATHER_LEAVES_HPP_ */
