/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _ENVIRON_MANAGER_HPP_
#define _ENVIRON_MANAGER_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "World/Environ/Types.hpp"

#include "World/Environ/Rain.hpp"
#include "World/Environ/Moon.hpp"
#include "World/Environ/Stars.hpp"
#include "World/Environ/Sky.hpp"
#include "World/Environ/Leaves.hpp"
#include "World/Environ/Clouds.hpp"
#include "World/Environ/Birds.hpp"
#include "World/Environ/Lightnings.hpp"

#include <boost/serialization/singleton.hpp>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <map>

class EnvironManager : public boost::serialization::singleton<EnvironManager>
{
    private:
        typedef std::map<EnvironTypes, EnvironObject*> MapType;
        MapType mObjectsMap;
        float mWindPower;

        bool mNight, mEnvironEnabled;
        int mEnvironTime[3]; // 0 - hours; 1 - minutes; 2 - seconds
        float mTotalSec;

    public:
        void initEnviron();
        ~EnvironManager();
        bool isNight();

        void setEnvironEnabled(bool state);
        bool getEnvironEnabled();

        void setWindPower(float _power);
        float getWindPower();

        void setLimit(EnvironTypes type, int limit);
        int getLimit(EnvironTypes type);

        void enableType(bool state, EnvironTypes type, int limit = -1);
        EnvironObject *getTypeHandle(EnvironTypes type);

        bool getTypeEnabled(EnvironTypes type);

        void setEnvironTime(int _hours, int _minutes, int _seconds);
        int *getEnvironTime();

        void update();
};

inline EnvironManager &environManager() { return EnvironManager::get_mutable_instance(); }

#endif /* _ENVIRON_MANAGER_HPP_ */
