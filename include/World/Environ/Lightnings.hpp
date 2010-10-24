#ifndef _ENVIRON_LIGHTNINGS_HPP_
#define _ENVIRON_LIGHTNINGS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"

#include "Core/ApplicationManager.hpp"
#include "World/Environ/Object.hpp"
#include "World/Environ/Types.hpp"

class Lightnings : public EnvironObject
{
    private:
        CL_Image mLightningImage;

        const int mProbability;
        bool mAnimation;
        float mTimer;

        CL_GraphicContext mGC;

    public:
        Lightnings();
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _ENVIRON_LIGHTNINGS_HPP_ */
