#ifndef _WORLD_MANAGER_HPP_
#define _WORLD_MANAGER_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"

#include "World/Weather/Rain.hpp"
#include "World/Weather/Moon.hpp"
#include "World/Weather/Stars.hpp"
#include "World/Weather/Sky.hpp"

#include <boost/serialization/singleton.hpp>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

#define worldManager (WorldManager::get_mutable_instance())
#define worldManagerConst (WorldManager::get_const_instance())

class WorldManager : public boost::serialization::singleton<WorldManager>
{
    private:
        float mWindPower;

        bool mNight;
        int mWorldTime[3]; // 0 - hours; 1 - minutes; 2 - seconds
        float mTotalSec;

        Rain *mRainHandle;
        Moon *mMoonHandle;
        Stars *mStarsHandle;
        Sky *mSkyHandle;

        bool mRainEnabled;
        bool mMoonEnabled;
        bool mStarsEnabled;
        bool mSkyEnabled;

    public:
        void initWorld();
        ~WorldManager();

        void setWindPower(float _power);
        float getWindPower();

        void setDropLimit(float maxDrops);
        float getDropLimit();

        float getMoonAngle();

        void setWorldTime(int _hours, int _minutes, int _seconds);

        void enableRain(bool state, int _maxDrops = 150);
        void enableMoon(bool state, float _scaleX = 1, float _scaleY = 1);
        void enableStars(bool state);
        void enableSky(bool state);

        int *getWorldTime();
        bool isNight();

        void update();
};

#endif /* _WORLD_MANAGER_HPP_ */
