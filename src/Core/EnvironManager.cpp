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
    mObjectsMap.clear();
}

void EnvironManager::setWindPower(float _power)
{
    mWindPower = Meters2Pixels(_power);
}

float EnvironManager::getWindPower()
{
    return Pixels2Meters(mWindPower);
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

void EnvironManager::update()
{
    float elapsed = appManager.getElapsed();
    float gameSeconds = GameSeconds(elapsed);
    elapsed /= 1000.f;

    mTotalSec += gameSeconds;
    mEnvironTime[2] += gameSeconds;
    float totalHours = mTotalSec/60.0/60.0;

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
        it->second->update(mWindPower, elapsed, totalHours);
    }
}

void EnvironManager::enableType(bool state, EnvironTypes type, int limit)
{
    // If the type is already exists in map
    if (mObjectsMap.find(type) != mObjectsMap.end())
    {
        mObjectsMap[type]->setEnabled(state);
    }
    else
    {
        EnvironObject *temp;
        switch (type)
        {
            case Environ_Sky:    temp = new Sky();    break;
            case Environ_Stars:  temp = new Stars();  break;
            case Environ_Moon:   temp = new Moon();   break;
            case Environ_Clouds: temp = new Clouds(); break;
            case Environ_Rain:   temp = new Rain();   break;
            case Environ_Leaves: temp = new Leaves(); break;
            case Environ_Birds:  temp = new Birds();  break;
            case Environ_Lightnings:  temp = new Lightnings();  break;
        }
        temp->setEnabled(state);
        mObjectsMap.insert(MapType::value_type(type, temp));
    }

    if (limit != -1) mObjectsMap[type]->setLimit(limit);
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
    {
        return false;
    }
    else
    {
        return mObjectsMap[type]->getEnabled();
    }

    return false;
}
