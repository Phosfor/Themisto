/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _ENVIRON_BIRDS_HPP_
#define _ENVIRON_BIRDS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "World/Environ/Object.hpp"

struct BirdData
{
    float x, y, x_speed, scale, side, timeout;
    CL_SpriteDescription mImageDesc;
    CL_Sprite mBirdImage;
};

class Birds : public EnvironObject
{
    private:
        std::vector<BirdData> mBirds;
        int mProbability; // The probability that bird will begin to fly current frame
        bool mFirstTime;

        CL_GraphicContext mGC;

        void processBirds(CL_GraphicContext &gc, int width, int height, int i);

    public:
        Birds(int maxBirds = 1);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _ENVIRON_BIRDS_HPP_ */
