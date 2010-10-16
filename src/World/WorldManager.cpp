#include "World/WorldManager.hpp"

void WorldManager::initWorld()
{
    mWindPower = 0.0f;
    memset(mWorldTime, 0, sizeof(mWorldTime));
    mNight = false;
    mTotalSec = 0.0f;

    //mMoonHandle = new Moon("moon_small2.png", 1, 1);
    mMoonHandle = new Moon("SkyX_Moon.png", 1, 1);
    mRainHandle = new Rain();
    mStarsHandle = new Stars();
    mSkyHandle = new Sky();
    mLeavesHandle = new Leaves();

    mMoonEnabled = false;
    mRainEnabled = false;
    mStarsEnabled = false;
    mSkyEnabled = false;
    mLeavesEnabled =false;
}

WorldManager::~WorldManager()
{
    delete mRainHandle;
    delete mMoonHandle;
    delete mStarsHandle;
    delete mSkyHandle;
    delete mLeavesHandle;
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

void WorldManager::enableSky(bool state)
{
    mSkyEnabled = state;
}

void WorldManager::enableLeaves(bool state, int _maxLeaves)
{
    if (_maxLeaves != -1) mLeavesHandle->setLeafLimit(_maxLeaves);
    mLeavesEnabled = state;
}

void WorldManager::update()
{
    float gameSeconds = GameSeconds(appManager.getElapsed());
    mTotalSec += gameSeconds;
    mWorldTime[2] += gameSeconds;

    if (mWorldTime[2] >= 60)
    {
        mWorldTime[2] -= 60;
        mWorldTime[1]++;

        if (mWorldTime[1] >= 60)
        {
            mWorldTime[1] -= 60;
            mWorldTime[0]++;

            if (mWorldTime[0] >= 24)
            {
                mWorldTime[0] = mWorldTime[1] = mWorldTime[2] = mTotalSec = 0;
            }
        }
    }

    if (mSkyEnabled) mSkyHandle->update(mTotalSec/60.0/60.0);
    if (mStarsEnabled) mStarsHandle->update(mTotalSec/60.0/60.0);
    if (mMoonEnabled) mMoonHandle->update(mTotalSec/60.0/60.0);

    if (mLeavesEnabled) mLeavesHandle->update(mWindPower);
    if (mRainEnabled) mRainHandle->update(mWindPower);
}

float WorldManager::getDropLimit()
{
    return mRainHandle->getDropLimit();
}

void WorldManager::setDropLimit(float maxDrops)
{
    mRainHandle->setDropLimit(maxDrops);
}

float WorldManager::getMoonAngle()
{
    return mMoonHandle->getMoonAngle();
}
