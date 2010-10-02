#include "World/Weather/Moon.hpp"

Moon::Moon(const string &imagePath, float _scaleX, float _scaleY)
{
    string media = utils.getMediaFolder();
    CL_GraphicContext gc = appManager.getGraphic();
    mMoon = CL_Sprite(appManager.getGraphic(), utils.getMediaFolder() + "/" + imagePath);

    // Apply image scaling and linear filtrating (to make sprite smooth)
    mMoon.set_scale(_scaleX, _scaleY);
    mMoon.set_linear_filter(true);

    // Find coordinated of the Moon center
    mMoonCenterX = mMoon.get_width()*_scaleX / 2;
    mMoonCenterY = mMoon.get_height()*_scaleY / 2;

    mGlobalTime = 0;
    mRadius = 600;           // The orbit radius is about 600 pixels
    mAngle = Deg2Rad(-85);   // Start Moon position (85 degrees)

    // Center (x;y) of the Moon orbit
    mCenterX = 0;
    mCenterY = appManager.getWindow().get_geometry().get_height();
}

void Moon::setScale(float _scaleX, float _scaleY)
{
    mMoon.set_scale(_scaleX, _scaleY);

    mMoonCenterX = mMoon.get_width()*_scaleX / 2;
    mMoonCenterY = mMoon.get_height()*_scaleY / 2;
}

void Moon::update()
{
    float x_local = (mRadius * cos(mAngle)) * 1.3;   // X position of the Moon
    float y_local = (mRadius * sin(mAngle)) * 1.1;   // Y position of the Moon

    mMoon.draw(appManager.getGraphic(), 
            x_local + mCenterX - mMoonCenterX,
            y_local + mCenterY - mMoonCenterY);

    mGlobalTime += appManager.getElapsed();

    // Increase angle each 200 ms
    if (mGlobalTime >= 75)
    {
        mGlobalTime = 0;
        if (mAngle <= Deg2Rad(0)) mAngle+=0.0005;
        /*TODO: Do something when moon is hided */
    }
}
