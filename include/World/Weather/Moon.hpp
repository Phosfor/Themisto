#ifndef _WEATHER_MOON_HPP_
#define _WEATHER_MOON_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"

#include "Core/ApplicationManager.hpp"
#include "Core/GuiManager.hpp"
#include <boost/foreach.hpp>

using namespace std;

class Moon
{
    private:
        // Moon graphic representing
        CL_Sprite mMoon;
        CL_Colorf mMoonColor;
        float mG, mB;
        float mScaleX, mScaleY;

        // Moon ellipse orbit
        int mRadius;
        float mCenterX, mCenterY;
        float mAngle;         // The angle where Moon is currently located

        bool mRenderMoon;
        bool mNearEscaping; // True when moon is going out of the screen

        CL_GraphicContext mGC;

    public:
        Moon(const string &imagePath = "moon.png", float _scaleX = 1, float _scaleY = 1);
        void setScale(float _scaleX, float _scaleY);
        bool nearEscape();
        void setNearEscape(bool state);
        void update(int hours);
};

#endif /* _WEATHER_MOON_HPP_ */
