#include "Core/ApplicationManager.hpp"

ApplicationManager::~ApplicationManager()
{
    delete mWindow;
}

ApplicationManager::ApplicationManager():
    mRunning(true), mLastTime(CL_System::get_time()), mCurrentTime(0), mTimeDifference(0), mDeltaTimeMs(0) {}

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

CL_DisplayWindow &ApplicationManager::getWindow()
{
    return *mWindow;
}

void ApplicationManager::initWindow(const string &title)
{
    // Initializating part
    short width = configManager.getValue<int>("window.width", 640);
    short height = configManager.getValue<int>("window.height", 480);
    bool fullscreen = configManager.getValue<bool>("window.fullscreen", false);

    CL_DisplayWindowDescription desc(title);
    desc.set_fullscreen(fullscreen);
    desc.set_size(CL_Size(width, height), false);
    mWindow = new CL_DisplayWindow(desc);
}
