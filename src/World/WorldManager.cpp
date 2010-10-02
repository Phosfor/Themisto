#include "World/WorldManager.hpp"

template<> WorldManager* ISingleton<WorldManager>::mInstance = 0;

WorldManager::WorldManager()
{
    mWindPower = 0.0;
    memset(mWorldTime, 0, sizeof(mWorldTime));
    mNight = false;

    mMoonHandle = new Moon("moon.png", 0.4, 0.4);
    mRainHandle = new Rain();

    mMoonEnabled = false;
    mRainEnabled = false;
}

WorldManager::~WorldManager()
{
    delete mRainHandle;
    delete mMoonHandle;
}

void WorldManager::setWindPower(float _power)
{
    mWindPower = _power;
}

int *WorldManager::getWorldTime()
{
    return mWorldTime;
}

bool WorldManager::isNight()
{
    return mNight;
}

void WorldManager::setWorldTime(int _hours, int _minutes, int _seconds)
{
    mWorldTime[0] = _hours;
    mWorldTime[1] = _minutes;
    mWorldTime[2] = _seconds;
}

void WorldManager::enableRain(bool state, int _maxDrops)
{
    mRainHandle->setDropLimit(_maxDrops);
    mRainEnabled = state;
}

void WorldManager::enableMoon(bool state, float _scaleX, float _scaleY)
{
    mMoonHandle->setScale(_scaleX, _scaleY);
    mMoonEnabled = state;
}

void WorldManager::update()
{
    if (mMoonEnabled) mMoonHandle->update();
    if (mRainEnabled) mRainHandle->update(mWindPower);

    mWorldTime[2] += appManager.getElapsed();
    if (mWorldTime[2] >= 60)
    {
        mWorldTime[2] = 0;
        mWorldTime[1]++;

        if (mWorldTime[1] >= 60)
        {
            mWorldTime[1] = 0;
            mWorldTime[0]++;
        }
    }
}
