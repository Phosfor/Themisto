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

#include "Core/EnvironManager.hpp"

//#include "World/Environ/Rain.hpp"
//#include "World/Environ/Moon.hpp"
#include "World/Environ/Stars.hpp"
//#include "World/Environ/Sky.hpp"
#include "World/Environ/Leaves.hpp"
//#include "World/Environ/Lightnings.hpp"
//#include "World/Environ/Foreground.hpp"
#include "World/Environ/Objects.hpp"

// Abstract base
#include "World/Environ/Object.hpp"

void EnvironManager::initEnviron()
{
    mWindPower = 0.0f;
    memset(mEnvironTime, 0, sizeof(mEnvironTime));
    mNight = false;
    mTotalSec = 0.0f;
}

EnvironManager::~EnvironManager()
{
    mObjectsMap.clear();
}

void EnvironManager::setWindPower(float _power)
{
    mWindPower = Meters2Pixels(_power);
}

float EnvironManager::getWindPower()
{
    return mWindPower;
}

int *EnvironManager::getEnvironTime()
{
    return mEnvironTime;
}

bool EnvironManager::isNight()
{
    return mNight;
}

void EnvironManager::setEnvironTime(int _hours, int _minutes, int _seconds)
{
    mEnvironTime[0] = _hours;
    mEnvironTime[1] = _minutes;
    mEnvironTime[2] = _seconds;
}

float EnvironManager::getAbsTime()
{
    return mTotalHours;
}

void EnvironManager::update()
{
    float elapsed = appManager().getElapsed();
    float gameSeconds = GameSeconds(elapsed);
    elapsed *= 0.001f;

    mTotalSec += gameSeconds;
    mEnvironTime[2] += gameSeconds;
    mTotalHours = mTotalSec/3600.0f;

    if (mEnvironTime[2] >= 60)
    {
        mEnvironTime[2] -= 60;
        mEnvironTime[1]++;

        if (mEnvironTime[1] >= 60)
        {
            mEnvironTime[1] -= 60;
            mEnvironTime[0]++;

            if (mEnvironTime[0] >= 24)
                mEnvironTime[0] = mEnvironTime[1] = mEnvironTime[2] = mTotalSec = 0;
        }
    }

    // Update all environ objects
    for (MapType::const_iterator it = mObjectsMap.begin(); it != mObjectsMap.end(); ++it)
    {
        it->second->update(elapsed);
    }
}

void EnvironManager::enableType(bool state, EnvironTypes type, float limit)
{
    // If the type is already exists in map
    if (mObjectsMap.find(type) != mObjectsMap.end())
    {
        mObjectsMap[type]->setEnabled(state);
    }
    else
    {
        EnvironObject *temp = NULL;
        std::cout << "Type: " << type << "\n";
        switch (type)
        {
            //case Environ_Sky:    temp = new Sky();    break;
            case Environ_Stars:  temp = new Stars();  break;
            //case Environ_Moon:   temp = new Moon();   break;
            //case Environ_Rain:   temp = new Rain();   break;
            case Environ_Leaves: temp = new Leaves(); break;
            case Environ_Objects:  temp = new Objects();  break;
            //case Environ_Foreground:  temp = new Foreground();  break;
            //case Environ_Lightnings:  temp = new Lightnings();  break;
            default: throw CL_Exception("Unknown environ type");
        }
        temp->setEnabled(state);
        mObjectsMap.insert(MapType::value_type(type, temp));
    }

    if (limit != -1)
    {
        float area = Pixels2Meters(ScreenResolutionX) * Pixels2Meters(ScreenResolutionY);
        mObjectsMap[type]->setLimit(limit * area);
    }
}

EnvironObject *EnvironManager::getTypeHandle(EnvironTypes type)
{
    if (mObjectsMap.find(type) == mObjectsMap.end())
    {
        LOG("You was trying to get nonexistent object handle with type id: " + type);
        return NULL;
    }
    else
    {
        return mObjectsMap[type];
    }
}

void EnvironManager::setLimit(EnvironTypes type, int limit)
{
    mObjectsMap[type]->setLimit(limit);
}

int EnvironManager::getLimit(EnvironTypes type)
{
    return mObjectsMap[type]->getLimit();
}

bool EnvironManager::getTypeEnabled(EnvironTypes type)
{
    if (mObjectsMap.find(type) == mObjectsMap.end())
        return false;
    else
        return mObjectsMap[type]->getEnabled();

    return false;
}
