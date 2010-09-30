#include "Managers/ApplicationManager.hpp"

ApplicationManager::ApplicationManager():
    mLastTime(CL_System::get_time()), mCurrentTime(0), mTimeDifference(0), mDeltaTimeMs(0), mRunning(true)  {}

void ApplicationManager::frameStarted()
{
    mCurrentTime = CL_System::get_time();
    mTimeDifference = mCurrentTime - mLastTime;
    if (mTimeDifference > 1000) mTimeDifference = 1000;
    mDeltaTimeMs = static_cast<float>(mTimeDifference);
    mLastTime = mCurrentTime;
}

void ApplicationManager::frameEnded()
{
    mCurrentTime = CL_System::get_time();
    const int mainLoopRate = 10; // 100 hz
    int sleepTime = mainLoopRate - (mCurrentTime - mLastTime);
    if (sleepTime > 0) CL_System::sleep(sleepTime);
}

int ApplicationManager::getFps()
{
    return mFps;
}

float ApplicationManager::getElapsed()
{
    return mDeltaTimeMs;
}

bool ApplicationManager::getRunning()
{
    return mRunning;
}

void ApplicationManager::setRunning(bool state)
{
    mRunning = state;
}

CL_GraphicContext &ApplicationManager::getGraphic()
{
    return mWindow->get_gc();
}

CL_InputDevice &ApplicationManager::getKeyboard()
{
    return mWindow->get_ic().get_keyboard();
}

CL_DisplayWindow &ApplicationManager::getWindow()
{
    return *mWindow.get();
}

void ApplicationManager::initWindow(const CL_DisplayWindowDescription &desc)
{
    mWindow = CL_SharedPtr<CL_DisplayWindow>(new CL_DisplayWindow(desc));
}
