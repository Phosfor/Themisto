/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _ENVIRON_LIGHTNINGS_HPP_
#define _ENVIRON_LIGHTNINGS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"

#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "World/Environ/Object.hpp"
#include "World/Environ/Types.hpp"
#include "World/Environ/Clouds.hpp"

class Lightnings : public EnvironObject
{
    private:
        Clouds *mCloudsHandle;

        CL_Pointf mPosition;
        std::vector<CL_Image> mLightningImages;

        const int mProbability;
        bool mAnimation;
        float mTimer, alpha;
        int mHandle;

        CL_GraphicContext mGC;

    public:
        Lightnings();
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _ENVIRON_LIGHTNINGS_HPP_ */
