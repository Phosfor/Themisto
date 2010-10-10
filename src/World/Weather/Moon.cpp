#include "World/Weather/Moon.hpp"

Moon::Moon(const string &imagePath, float _scaleX, float _scaleY)
{
    string media = utils.getMediaFolder();
    CL_GraphicContext gc = appManager.getGraphic();
    mMoon = CL_Sprite(appManager.getGraphic(), utils.getMediaFolder() + "/" + imagePath);

    mMoonColor = mMoon.get_color();
    mG = mB = 1.0f;
    mScaleX = mScaleY = 1.0f;

    mRenderMoon = true;
    mNearEscaping = false;

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

void Moon::update(int hours)
{
    if (!mRenderMoon) return;

    float moonX = (mRadius * cos(mAngle)) * 1.3 + mCenterX;   // X position of the Moon
    float moonY = (mRadius * sin(mAngle)) * 1.1 + mCenterY;   // Y position of the Moon

    // Draw simple moon
    if (mAngle > Deg2Rad(-20))
    {
        mNearEscaping = true;
        if (mG > 0.85f ) mG -= 0.0002;
        if (mB > 0.75f) mB -= 0.0004;
        if (mScaleX < 1.15) mScaleX += 0.00018;
        if (mScaleY < 1.05) mScaleY += 0.0001;
    }

    if (mNearEscaping)
    {
        mMoon.set_scale(mScaleX, mScaleY);
        mMoon.set_color(CL_Colorf(1.0f, mG, mB));
    }
    mMoon.draw(mGC, moonX, moonY);

    float mRadIncrease = (Deg2Rad(90) * GameSeconds(appManager.getElapsed())) / (12.0*60.0*60.0);

    if (mAngle <= Deg2Rad(5))
        mAngle+=mRadIncrease;
    else
        mRenderMoon = false;
}

bool Moon::nearEscape()
{
    return mNearEscaping;
}

void Moon::setNearEscape(bool state)
{
    mNearEscaping = state;
    // TODO: If true, move Moon to the -90 degrees
}
