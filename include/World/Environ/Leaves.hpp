/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

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
        std::vector<LeafData> mLeaves;
        bool mFirstTime;

        CL_GraphicContext mGC;

        void processLeaves(CL_GraphicContext &gc, float windPower, int i);

    public:
        Leaves(int maxLeaves = 5);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _WEATHER_LEAVES_HPP_ */
