#include "Managers/FrameManager.hpp"

FrameManager::FrameManager():
    mFps(0), mStartTime(CL_System::get_time()), mFrames(0), mElapsed(0), mRunning(true) { }

void FrameManager::frameStarted()
{
    mElapsed = CL_System::get_time() - mStartTime;
    mFrames++;
    int currentTime = CL_System::get_time();
    if (mStartTime == 0) {
        mStartTime = currentTime;
    } else {
        int delta = currentTime - mStartTime;
        if (delta < 0 || delta > 2000)
        {
            if (delta > 0) mFps = (mFrames*1000) / delta;
            mFrames = 0;
            mStartTime = currentTime;
        }
    }
}

int FrameManager::getFps()
{
    return mFps;
}

float FrameManager::getElapsed()
{
    return mElapsed;
}

bool FrameManager::getRunning()
{
    return mRunning;
}

void FrameManager::setRunning(bool state)
{
    mRunning = state;
}
