#ifndef _WEATHER_MOON_HPP_
#define _WEATHER_MOON_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"

#include "Core/ApplicationManager.hpp"
#include <boost/foreach.hpp>

using namespace std;

class Moon
{
    private:
        // Moon graphic representing
        CL_Sprite mMoon;
        int mMoonCenterX, mMoonCenterY;

        // Moon ellipse orbit
        int mRadius;
        float mCenterX, mCenterY;

        float mGlobalTime;    // Time for one step Moon animation
        float mAngle;         // The angle where Moon is currently located

        CL_GraphicContext mGC;

        CL_ProgramObject mProgramBlurH;
        CL_ProgramObject mProgramBlurV;
        CL_ProgramObject mProgramBloom;

        CL_Texture mBlurTexture;
        CL_FrameBuffer mBlurBuf;

        CL_Texture mBloomTexture;
        CL_FrameBuffer mBloomBuf;

        CL_Texture mSceneTexture;
        CL_FrameBuffer mSceneBuf;

        void makeBloomHandle();
        void makeBlurHandleH();
        void makeBlurHandleV();

        void renderMoon(CL_ProgramObject &program);

        /*TODO: Global game-time (day) management */

    public:
        Moon(const string &imagePath = "moon.png", float _scaleX = 1, float _scaleY = 1);
        void setScale(float _scaleX, float _scaleY);
        void update();

};

#endif /* _WEATHER_MOON_HPP_ */
