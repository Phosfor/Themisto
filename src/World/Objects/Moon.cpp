/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "World/Objects/Moon.hpp"

Moon::Moon()
{
    mType = MoonObject;
    mGC = appManager().getGraphic();

    std::string media = utils().getMediaFolder();
    CL_GraphicContext gc = appManager().getGraphic();
    mMoon = resourceManager().getImage("Moons", "0");
    mMoonColor = mMoon.get_color();
    mG = mB = 1.0f;

    mRenderMoon = true;

    mMoon.set_alignment(origin_center_right);

    mRadius = Meters2Pixels(7);  // The orbit radius is about 7 game meters
    mAngle = Deg2Rad(-90);       // Start Moon position

    // Center (x;y) of the Moon orbit
    mCenterX = 0;
    mCenterY = appManager().getWindow().get_geometry().get_height();

    mPosX = mPosY = 0;
}

Moon::~Moon() { }

void Moon::updateVisual(float newX, float newY)
{
    if (!mRenderMoon) return;
    mMoon.draw(mGC, mPosX, mPosY);
}

void Moon::update(float elapsed)
{
    float globalTime = environManager().getAbsTime();

    // Set moon back
    if (!mRenderMoon && (globalTime == 0.0f || globalTime <= 0.3f ))
    {
        mRenderMoon = true;
        mAngle = Deg2Rad(-90);
        mMoon.set_color(CL_Colorf::white);
    }
    if (!mRenderMoon) return;

    mPosX = (mRadius * cos(mAngle)) * 1.3 + mCenterX;   // X position of the Moon
    mPosY = (mRadius * sin(mAngle)) * 1.1 + mCenterY;   // Y position of the Moon

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

    float mRadIncrease = (Deg2Rad(95) * GameSeconds(elapsed * 1000.0)) / (12.0*60.0*60.0);

    if (mAngle <= Deg2Rad(10))
        mAngle+=mRadIncrease;
    else
        mRenderMoon = false;
}

void Moon::setScale(float _scaleX, float _scaleY)
{
    mMoon.set_scale(_scaleX, _scaleY);
}

float Moon::getMoonAngle()
{
    return Rad2Deg(mAngle);
}

CL_Rectf Moon::getRectangle()
{
    return CL_Rectf(mPosX - mMoon.get_width(), mPosY - mMoon.get_height()/2.0f,
            mPosX, mPosY + mMoon.get_height()/2.0f);
}

CL_Pointf Moon::getPosition()
{
    return CL_Pointf(mPosX, mPosY);
}

void Moon::setPosition(CL_Pointf newPos)
{
}

boost::shared_ptr<Object> Moon::ParseMoon(CL_DomElement *node, std::string &desc)
{
    return boost::shared_ptr<Object>(new Moon);
}
