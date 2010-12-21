/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Environ/Moon.hpp"

Moon::Moon()
{
    std::string media = utils().getMediaFolder();
    CL_GraphicContext gc = appManager().getGraphic();
    mMoon = CL_Sprite(gc, resourceManager().texturePath("Moons", "0"));

    float koef = Meters2Pixels(9.0) / mMoon.get_width();
    mMoon.set_scale(koef, koef);

    mMoonColor = mMoon.get_color();
    mG = mB = 1.0f;

    mRenderMoon = true;

    mMoon.set_alignment(origin_center_right);

    mRadius = Meters2Pixels(15); // The orbit radius is about 15 game meters
    mAngle = Deg2Rad(-90);       // Start Moon position

    // Center (x;y) of the Moon orbit
    mCenterX = 0;
    mCenterY = appManager().getWindow().get_geometry().get_height();

    mGC = appManager().getGraphic();
}

void Moon::setScale(float _scaleX, float _scaleY)
{
    mMoon.set_scale(_scaleX, _scaleY);
}

void Moon::update(float windPower, float elapsed, float globalTime)
{
    // Set moon back
    if (!mRenderMoon && (globalTime == 0.0f || globalTime <= 0.3f ))
    {
        mRenderMoon = true;
        mAngle = Deg2Rad(-90);
        mMoon.set_color(CL_Colorf::white);
    }
    if (!mRenderMoon) return;

    float moonX = (mRadius * cos(mAngle)) * 1.3 + mCenterX;   // X position of the Moon
    float moonY = (mRadius * sin(mAngle)) * 1.1 + mCenterY;   // Y position of the Moon

    if (globalTime <= 10.0f) {
        t = 0.0f;
    } else if (globalTime >= 12.0f) {
        t = 1.0f;
    } else {
        t = (globalTime - 10.0f) / (12.0f - 10.0f);

        mG = (1-t)*0.25 + 0.75;
        mB = (1-t)*0.45 + 0.45;

        mMoon.set_color(CL_Colorf(1.0f, mG, mB));
    }

    mMoon.draw(mGC, moonX, moonY);

    float mRadIncrease = (Deg2Rad(95) * GameSeconds(elapsed * 1000.0)) / (12.0*60.0*60.0);

    //cout << "game seconds: " << GameSeconds(appManager.getElapsed()) << "\n";
    //cout << "mRadIncrease: " << mRadIncrease << "\n";

    if (mAngle <= Deg2Rad(10))
        mAngle+=mRadIncrease;
    else
        mRenderMoon = false;

    //cout << "Angle: " << Rad2Deg(mAngle) << "\n";
}

float Moon::getMoonAngle()
{
    return Rad2Deg(mAngle);
}
