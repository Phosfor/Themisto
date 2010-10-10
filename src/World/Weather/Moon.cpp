#include "World/Weather/Moon.hpp"

Moon::Moon(const string &imagePath, float _scaleX, float _scaleY)
{
    string media = utils.getMediaFolder();
    CL_GraphicContext gc = appManager.getGraphic();
    mMoon = CL_Sprite(appManager.getGraphic(), utils.getMediaFolder() + "/" + imagePath);

    mMoonColor = mMoon.get_color();
    mG = 1.0f;
    mB = 1.0f;
    mScaleX = 1.0f;
    mScaleY = 1.0f;
    mRenderMoon = true;

    // Apply image scaling and linear filtrating (to make sprite smooth)
    mMoon.set_scale(_scaleX, _scaleY);
    mMoon.set_linear_filter(true);

    // Find coordinated of the Moon center
    mMoonCenterX = mMoon.get_width()*_scaleX / 2;
    mMoonCenterY = mMoon.get_height()*_scaleY / 2;
    //mMoon.set_alignment(origin_center);

    mRadius = Meters2Pixels(15); // The orbit radius is about 600 pixels
    mAngle = Deg2Rad(-90);       // Start Moon position (85 degrees)
    mColorOffset = 0.0;

    // Center (x;y) of the Moon orbit
    mCenterX = 0;
    mCenterY = appManager.getWindow().get_geometry().get_height();

    mGC = appManager.getGraphic();
}

void Moon::setScale(float _scaleX, float _scaleY)
{
    mMoon.set_scale(_scaleX, _scaleY);

    mMoonCenterX = mMoon.get_width()*_scaleX / 2;
    mMoonCenterY = mMoon.get_height()*_scaleY / 2;
}

void Moon::update(int hours)
{
    if (!mRenderMoon) return;

    float x_local = (mRadius * cos(mAngle)) * 1.3;   // X position of the Moon
    float y_local = (mRadius * sin(mAngle)) * 1.1;   // Y position of the Moon

    float moonX = x_local + mCenterX - mMoonCenterX;
    float moonY = y_local + mCenterY - mMoonCenterY;

    // Draw simple moon
    if (mAngle > Deg2Rad(-20))
    {
        if (mG > 0.85f ) mG -= 0.0002;
        if (mB > 0.75f) mB -= 0.0004;
        if (mScaleX < 1.15) mScaleX += 0.00018;
        if (mScaleY < 1.05) mScaleY += 0.0001;
    }

    mMoon.set_scale(mScaleX, mScaleY);
    mMoon.set_color(CL_Colorf(1.0f, mG, mB));
    mMoon.draw(mGC, moonX, moonY);
    mMoon.set_color(CL_Colorf(1.0f, 1.0f, 1.0f));

    // TODO: remove deg2rad(90) with number
    float mRadIncrease = (Deg2Rad(90) * GameSeconds(appManager.getElapsed())) / (12.0*60.0*60.0);

    if (mAngle <= Deg2Rad(5))
        mAngle+=mRadIncrease;
    else
        mRenderMoon = false;
}
