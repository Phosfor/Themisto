/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _FRAME_MANAGER_H_
#define _FRAME_MANAGER_H_

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include <boost/serialization/singleton.hpp>
#include "Core/ConfigManager.hpp"

#define appManager (ApplicationManager::get_mutable_instance())
#define appManagerConst (applicationManager::get_const_instance())

class ApplicationManager : public boost::serialization::singleton<ApplicationManager>
{
    private:
        int mFps, mStartTime, mFrames;
        float mElapsed;
        bool mRunning;
        CL_DisplayWindow *mWindow;

        unsigned int mLastTime, mCurrentTime;
        int mTimeDifference;
        float mDeltaTimeMs;

    public:
        ~ApplicationManager();
        ApplicationManager();
        void initWindow(const string &title);

        void frameStarted();
        void frameEnded();

        int getFps();
        float getElapsed();

        bool getRunning();
        void setRunning(bool state);

        CL_GraphicContext &getGraphic();
        CL_DisplayWindow &getWindow();
};

#endif /* _FRAME_MANAGER_H_ */
