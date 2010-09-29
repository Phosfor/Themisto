#ifndef _FRAME_MANAGER_H_
#define _FRAME_MANAGER_H_

#include <ClanLib/core.h>
#include <boost/serialization/singleton.hpp>
#include <ClanLib/display.h>

#define appManager (ApplicationManager::get_mutable_instance())
#define appManagerConst (applicationManager::get_const_instance())

class ApplicationManager : public boost::serialization::singleton<ApplicationManager>
{
    private:
        int mFps, mStartTime, mFrames;
        float mElapsed;
        bool mRunning;

        CL_SharedPtr<CL_DisplayWindow> mWindow;

    public:
        ApplicationManager();
        void initWindow(const CL_DisplayWindowDescription &desc);

        void frameStarted();
        int getFps();
        float getElapsed();

        bool getRunning();
        void setRunning(bool state);

        CL_GraphicContext *getGraphic();
        CL_InputDevice *getKeyboard();
        CL_DisplayWindow *getWindow();
};

#endif /* _FRAME_MANAGER_H_ */
