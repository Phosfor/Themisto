#include "World/Environ/Moon.hpp"

Moon::Moon(const string &imagePath, float _scaleX, float _scaleY)
{
    string media = utils.getMediaFolder();
    CL_GraphicContext gc = appManager.getGraphic();
    mMoon = CL_Sprite(appManager.getGraphic(), utils.getMediaFolder() + "/" + imagePath);

    mMoonColor = mMoon.get_color();
    mG = mB = 1.0f;

    mRenderMoon = true;

    // Apply image scaling and linear filtrating (to make sprite smooth)
    mMoon.set_scale(_scaleX, _scaleY);
    mMoon.set_linear_filter(true);

    mMoon.set_alignment(origin_center);

    mRadius = Meters2Pixels(15); // The orbit radius is about 15 game meters
    mAngle = Deg2Rad(-90);       // Start Moon position (85 degrees)

    // Center (x;y) of the Moon orbit
    mCenterX = 0;
    mCenterY = appManager.getWindow().get_geometry().get_height();

    mGC = appManager.getGraphic();
}

void Moon::setScale(float _scaleX, float _scaleY)
{
    mMoon.set_scale(_scaleX, _scaleY);
}

void Moon::update(float hours)
{
    // Set moon back
    if (!mRenderMoon && hours >= 21.4f)
    {
        mRenderMoon = true;
        mAngle = Deg2Rad(-110);
        mMoon.set_color(CL_Colorf::white);
    }
    if (!mRenderMoon) return;

    float moonX = (mRadius * cos(mAngle)) * 1.3 + mCenterX;   // X position of the Moon
    float moonY = (mRadius * sin(mAngle)) * 1.1 + mCenterY;   // Y position of the Moon

    if (hours <= 9.0f) {
        t = 0.0f;
    } else if (hours >= 12.0f) {
        t = 1.0f;
    } else {
        t = (hours - 9.0f) / (12.0f - 9.0f);

        mG = (1-t)*0.25 + 0.75;
        mB = (1-t)*0.45 + 0.45;

        mMoon.set_color(CL_Colorf(1.0f, mG, mB));
    }

    mMoon.draw(mGC, moonX, moonY);

    float mRadIncrease = (Deg2Rad(90) * GameSeconds(appManager.getElapsed())) / (12.0*60.0*60.0);

    if (mAngle <= Deg2Rad(10))
        mAngle+=mRadIncrease;
    else
        mRenderMoon = false;
}

float Moon::getMoonAngle()
{
    return Rad2Deg(mAngle);
}
