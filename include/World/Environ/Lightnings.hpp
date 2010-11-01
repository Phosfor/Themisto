#ifndef _ENVIRON_LIGHTNINGS_HPP_
#define _ENVIRON_LIGHTNINGS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"

#include "Core/ApplicationManager.hpp"
#include "World/Environ/Object.hpp"
#include "World/Environ/Types.hpp"
#include "World/Environ/Clouds.hpp"

class Lightnings : public EnvironObject
{
    private:
        Clouds *mCloudsHandle;

        CL_Pointf mPosition;
        CL_Image mLightningImage[11];

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
