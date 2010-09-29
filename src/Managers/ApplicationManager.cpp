#include "Managers/ApplicationManager.hpp"

ApplicationManager::ApplicationManager():
    mFps(0), mStartTime(CL_System::get_time()), mFrames(0), mElapsed(0), mRunning(true) { }

void ApplicationManager::frameStarted()
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

int ApplicationManager::getFps()
{
    return mFps;
}

float ApplicationManager::getElapsed()
{
    return mElapsed;
}

bool ApplicationManager::getRunning()
{
    return mRunning;
}

void ApplicationManager::setRunning(bool state)
{
    mRunning = state;
}

CL_GraphicContext *ApplicationManager::getGraphic()
{
    return &mWindow->get_gc();
}

CL_InputDevice *ApplicationManager::getKeyboard()
{
    return &mWindow->get_ic().get_keyboard();
}

CL_DisplayWindow *ApplicationManager::getWindow()
{
    return mWindow.get();
}

void ApplicationManager::initWindow(const CL_DisplayWindowDescription &desc)
{
    mWindow = CL_SharedPtr<CL_DisplayWindow>(new CL_DisplayWindow(desc));
}
