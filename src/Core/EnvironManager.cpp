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

void EnvironManager::initEnviron()
{
    mWindPower = 0.0f;
    memset(mEnvironTime, 0, sizeof(mEnvironTime));
    mNight = false;
    mTotalSec = 0.0f;
}

EnvironManager::~EnvironManager()
{
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

    physicManager().update(elapsed);
    levelManager().updateVisual(elapsed);
}
