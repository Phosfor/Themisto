#ifndef _ENVIRON_MANAGER_HPP_
#define _ENVIRON_MANAGER_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"

#include "World/Environ/Rain.hpp"
#include "World/Environ/Moon.hpp"
#include "World/Environ/Stars.hpp"
#include "World/Environ/Sky.hpp"
#include "World/Environ/Leaves.hpp"
#include "World/Environ/Clouds.hpp"
#include "World/Environ/Birds.hpp"

#include <boost/serialization/singleton.hpp>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

#define environManager (EnvironManager::get_mutable_instance())
#define environManagerConst (EnvironManager::get_const_instance())

class EnvironManager : public boost::serialization::singleton<EnvironManager>
{
    private:
        float mWindPower;

        bool mNight;
        int mEnvironTime[3]; // 0 - hours; 1 - minutes; 2 - seconds
        float mTotalSec;

        Rain *mRainHandle;
        Moon *mMoonHandle;
        Stars *mStarsHandle;
        Sky *mSkyHandle;
        Leaves *mLeavesHandle;
        Clouds *mCloudsHandle;
        Birds *mBirdsHandle;

        bool mRainEnabled;
        bool mMoonEnabled;
        bool mStarsEnabled;
        bool mSkyEnabled;
        bool mLeavesEnabled;
        bool mCloudsEnabled;
        bool mBirdsEnabled;

    public:
        void initEnviron();
        ~EnvironManager();

        void setWindPower(float _power);
        float getWindPower();

        void setDropLimit(float maxDrops);
        int getDropLimit();

        float getMoonAngle();

        void setEnvironTime(int _hours, int _minutes, int _seconds);

        void enableRain(bool state, int _maxDrops = 150);
        void enableMoon(bool state, float _scaleX = 1, float _scaleY = 1);
        void enableStars(bool state);
        void enableSky(bool state);
        void enableLeaves(bool state, int _maxLeaves = -1);
        void enableClouds(bool state, int _maxClouds = -1);
        void enableBirds(bool state);

        int *getEnvironTime();
        bool isNight();

        void update();
};

#endif /* _ENVIRON_MANAGER_HPP_ */
