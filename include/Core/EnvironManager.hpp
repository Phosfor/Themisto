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

#ifndef _ENVIRON_MANAGER_HPP_
#define _ENVIRON_MANAGER_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "World/Environ/Types.hpp"

#include <boost/serialization/singleton.hpp>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <map>

class Rain;
class Moon;
class Stars;
class Sky;
class Leaves;
class Clouds;
class Birds;
class Lightnings;
class EnvironObject;
class Foreground;
class Objects;
class EnvironManager : public boost::serialization::singleton<EnvironManager>
{
    private:
        typedef std::map<EnvironTypes, EnvironObject*> MapType;
        MapType mObjectsMap;
        float mWindPower;

        bool mNight;
        int mEnvironTime[3]; // 0 - hours; 1 - minutes; 2 - seconds
        float mTotalSec;
        float mTotalHours;

    public:
        ~EnvironManager();

        void initEnviron();
        bool isNight();

        void setWindPower(float _power);
        float getWindPower();

        void setLimit(EnvironTypes type, int limit);
        int getLimit(EnvironTypes type);

        void enableType(bool state, EnvironTypes type, int limit = -1);
        EnvironObject *getTypeHandle(EnvironTypes type);

        bool getTypeEnabled(EnvironTypes type);

        void setEnvironTime(int _hours, int _minutes, int _seconds);
        int *getEnvironTime();

        float getAbsTime();

        void update();
};

inline EnvironManager &environManager() { return EnvironManager::get_mutable_instance(); }

#endif /* _ENVIRON_MANAGER_HPP_ */
