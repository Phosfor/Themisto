#include "World/WorldManager.hpp"

void WorldManager::initWorld()
{
    mWindPower = 0.0;
    memset(mWorldTime, 0, sizeof(mWorldTime));
    mNight = false;

    mMoonHandle = new Moon("moon_small2.png", 1, 1);
    mRainHandle = new Rain();
    mStarsHandle = new Stars();

    mMoonEnabled = false;
    mRainEnabled = false;
    mStarsEnabled = false;
}

WorldManager::~WorldManager()
{
    delete mRainHandle;
    delete mMoonHandle;
    delete mStarsHandle;
}

void WorldManager::setWindPower(float _power)
{
    mWindPower = Meters2Pixels(_power);
}

float WorldManager::getWindPower()
{
    return Pixels2Meters(mWindPower);
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

void WorldManager::enableStars(bool state)
{
    mStarsEnabled = state;
}

void WorldManager::enableMoon(bool state, float _scaleX, float _scaleY)
{
    mMoonHandle->setScale(_scaleX, _scaleY);
    mMoonEnabled = state;
}

void WorldManager::update()
{
    if (mStarsEnabled) mStarsHandle->update();
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

float WorldManager::getDropLimit()
{
    return mRainHandle->getDropLimit();
}

void WorldManager::setDropLimit(float maxDrops)
{
    mRainHandle->setDropLimit(maxDrops);
}
