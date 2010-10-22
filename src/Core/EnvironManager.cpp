#include "Core/EnvironManager.hpp"

void EnvironManager::initEnviron()
{
    mWindPower = 0.0f;
    memset(mEnvironTime, 0, sizeof(mEnvironTime));
    mNight = false;
    mTotalSec = 0.0f;

    //mMoonHandle = new Moon("moon_small2.png", 1, 1);
    mMoonHandle = new Moon("SkyX_Moon.png", 1, 1);
    mRainHandle = new Rain();
    mStarsHandle = new Stars();
    mSkyHandle = new Sky();
    mLeavesHandle = new Leaves();
    mCloudsHandle = new Clouds();
    mBirdsHandle = new Birds();

    mMoonEnabled = false;
    mRainEnabled = false;
    mStarsEnabled = false;
    mSkyEnabled = false;
    mLeavesEnabled =false;
    mCloudsEnabled =false;
    mBirdsEnabled = false;
}

EnvironManager::~EnvironManager()
{
    delete mRainHandle;
    delete mMoonHandle;
    delete mStarsHandle;
    delete mSkyHandle;
    delete mLeavesHandle;
    delete mCloudsHandle;
    delete mBirdsHandle;
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

void EnvironManager::enableRain(bool state, int _maxDrops)
{
    mRainHandle->setLimit(_maxDrops);
    mRainEnabled = state;
}

void EnvironManager::enableStars(bool state)
{
    mStarsEnabled = state;
}

void EnvironManager::enableMoon(bool state, float _scaleX, float _scaleY)
{
    mMoonHandle->setScale(_scaleX, _scaleY);
    mMoonEnabled = state;
}

void EnvironManager::enableSky(bool state)
{
    mSkyEnabled = state;
}

void EnvironManager::enableLeaves(bool state, int _maxLeaves)
{
    if (_maxLeaves != -1) mLeavesHandle->setLimit(_maxLeaves);
    mLeavesEnabled = state;
}

void EnvironManager::enableClouds(bool state, int _maxClouds)
{
    if (_maxClouds != -1) mCloudsHandle->setLimit(_maxClouds);
    mCloudsEnabled = state;
}

void EnvironManager::enableBirds(bool state)
{
    mBirdsEnabled = state;
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
            {
                mEnvironTime[0] = mEnvironTime[1] = mEnvironTime[2] = mTotalSec = 0;
            }
        }
    }

    if (mSkyEnabled) mSkyHandle->update(mWindPower, elapsed, totalHours);
    if (mStarsEnabled) mStarsHandle->update(mWindPower, elapsed, totalHours);
    if (mMoonEnabled) mMoonHandle->update(mWindPower, elapsed, totalHours);

    if (mCloudsEnabled) mCloudsHandle->update(mWindPower, elapsed, totalHours);
    if (mRainEnabled) mRainHandle->update(mWindPower, elapsed, totalHours);
    if (mLeavesEnabled) mLeavesHandle->update(mWindPower, elapsed, totalHours);
    if (mBirdsEnabled) mBirdsHandle->update(mWindPower, elapsed, totalHours);
}

int EnvironManager::getDropLimit()
{
    return mRainHandle->getLimit();
}

void EnvironManager::setDropLimit(float maxDrops)
{
    mRainHandle->setLimit(maxDrops);
}

float EnvironManager::getMoonAngle()
{
    return mMoonHandle->getMoonAngle();
}
