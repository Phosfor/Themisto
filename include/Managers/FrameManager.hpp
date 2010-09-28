#ifndef _FRAME_MANAGER_H_
#define _FRAME_MANAGER_H_

#include <ClanLib/core.h>
#include <boost/serialization/singleton.hpp>

#define frameManager (FrameManager::get_mutable_instance())
#define frameManagerConst (FrameManager::get_const_instance())

class FrameManager : public boost::serialization::singleton<FrameManager>
{
    private:
        int mFps, mStartTime, mFrames;
        float mElapsed;
        bool mRunning;

    public:
        FrameManager();
        void frameStarted();

        int getFps();
        float getElapsed();

        bool getRunning();
        void setRunning(bool state);
};

#endif /* _FRAME_MANAGER_H_ */
