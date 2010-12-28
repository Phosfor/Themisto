/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

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

void ApplicationManager::initWindow(const std::string &title)
{
    // Initializating part
    short width = configManager().getValue<int>("window.width", 640);
    short height = configManager().getValue<int>("window.height", 480);
    bool fullscreen = configManager().getValue<bool>("window.fullscreen", false);

    CL_DisplayWindowDescription desc(title);
    desc.set_fullscreen(fullscreen);
    desc.set_size(CL_Size(width, height), false);
    mWindow = new CL_DisplayWindow(desc);
}
