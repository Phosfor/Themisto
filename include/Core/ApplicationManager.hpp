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

#ifndef _APPLICATION_MANAGER_H_
#define _APPLICATION_MANAGER_H_

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include <boost/serialization/singleton.hpp>
#include "Core/ConfigManager.hpp"

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
        void initWindow(const std::string &title);

        void frameStarted();
        void frameEnded();

        int getFps();
        float getElapsed();

        bool getRunning();
        void setRunning(bool state);

        CL_GraphicContext &getGraphic();
        CL_DisplayWindow &getWindow();
};

inline ApplicationManager &appManager() { return ApplicationManager::get_mutable_instance(); }

#endif /* _APPLICATION_MANAGER_H_ */
