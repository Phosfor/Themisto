/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <boost/serialization/singleton.hpp>
#include <boost/shared_ptr.hpp>

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/PhysicManager.hpp"
#include "Core/LevelManager.hpp"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sstream>
#include <map>

class WorldManager : public boost::serialization::singleton<WorldManager>
{
    private:
        int mUniqueIDCounter;

        float mWindPower;

        bool mNight;
        int mEnvironTime[3]; // 0 - hours; 1 - minutes; 2 - seconds
        float mTotalSec;
        float mTotalHours;

    public:
        float mEnvironTemperature;

        void initWorld();
        WorldManager();
        std::string generateUniqueID();

        bool isNight();

        void setWindPower(float _power);
        float getWindPower();

        void setWorldTime(int _hours, int _minutes, int _seconds);
        int *getWorldTime();

        float getAbsTime();

        void update();
};

inline WorldManager &worldManager() { return WorldManager::get_mutable_instance(); }
