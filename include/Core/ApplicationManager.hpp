#ifndef _FRAME_MANAGER_H_
#define _FRAME_MANAGER_H_

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include <boost/serialization/singleton.hpp>

#define appManager (ApplicationManager::get_mutable_instance())
#define appManagerConst (applicationManager::get_const_instance())

class ApplicationManager : public boost::serialization::singleton<ApplicationManager>
{
    private:
        int mFps, mStartTime, mFrames;
        float mElapsed;
        bool mRunning;
        CL_SharedPtr<CL_DisplayWindow> mWindow;

        unsigned int mLastTime, mCurrentTime;
        int mTimeDifference;
        float mDeltaTimeMs;

    public:
        ApplicationManager();
        void initWindow(const CL_DisplayWindowDescription &desc/*, CL_GUIManager &gui*/);

        void frameStarted();
        void frameEnded();

        int getFps();
        float getElapsed();

        bool getRunning();
        void setRunning(bool state);

        CL_GraphicContext &getGraphic();
        CL_InputDevice &getKeyboard();
        CL_DisplayWindow &getWindow();
};

#endif /* _FRAME_MANAGER_H_ */
