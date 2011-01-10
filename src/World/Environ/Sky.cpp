/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

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

#include "World/Environ/Sky.hpp"

Sky::Sky()
   : EnvironObject()
{
    mColorTop = CL_Colorf(25/255.0f, 60/255.0f, 141/255.0f);
    mColorMiddle = CL_Colorf(54/255.0f, 119/255.0f, 198/255.0f);
    mColorBottom = CL_Colorf(83/255.0f, 178/255.0f, 234/255.0f);

    mColorTopConst = mColorTop;
    mColorMiddleConst = mColorMiddle;
    mColorBottomConst = mColorBottom;

    mColorTopTrans = CL_Colorf(53/255.0f, 23/255.0f, 104/255.0f);
    mColorMiddleTrans = CL_Colorf(54/255.0f, 85/255.0f, 198/255.0f);
    mColorBottomTrans = CL_Colorf(234/255.0f, 197/255.0f, 83/255.0f);

    t1 = t2 = t3 = 0.0f;
    mNight = false;

    mGC = appManager().getGraphic();

    quad1 = CL_Rectf(0, 0, mWindowWidth, mWindowHeight/2.0);
    quad2 = CL_Rectf(0, mWindowHeight/2.0, mWindowWidth, mWindowHeight);
}

void Sky::update(float elapsed)
{
    float globalTime = environManager().getAbsTime();

    // Make atmosphere scattering? [9; 11] globalTime
    if (globalTime <= 7.0f) {
        t1 = 0.0f;
    } else if (globalTime >= 10.0f) {
        t1 = 1.0f;
    } else {
        t1 = (globalTime - 7.0f) / (10.0f - 7.0f);

        // Make color transation of the sky bottom (to the scattering color)
        float r = t1 * (mColorBottomTrans.r - mColorBottomConst.r) + mColorBottomConst.r;
        float g = t1 * (mColorBottomTrans.g - mColorBottomConst.g) + mColorBottomConst.g;
        float b = t1 * (mColorBottomTrans.b - mColorBottomConst.b) + mColorBottomConst.b;

        mColorBottom = CL_Colorf(r, g, b);
    }

    if (globalTime <= 12.0f) {
        t2 = 0.0f;
    } else if (globalTime >= 14.0f) {
        t2 = 1.0f;
        mNight = true;
    } else {
        t2 = (globalTime - 12.0f) / (14.0f - 12.0f);

        // Get back bottom color after scattering
        float r = t2 * (mColorBottomConst.r - mColorBottomTrans.r) + mColorBottomTrans.r;
        float g = t2 * (mColorBottomConst.g - mColorBottomTrans.g) + mColorBottomTrans.g;
        float b = t2 * (mColorBottomConst.b - mColorBottomTrans.b) + mColorBottomTrans.b;

        mColorBottom = CL_Colorf(r, g, b);

        // After moon hiding the night becomes (make top part of the sky darker)
        r = t2 * (mColorTopTrans.r - mColorTopConst.r) + mColorTopConst.r;
        g = t2 * (mColorTopTrans.g - mColorTopConst.g) + mColorTopConst.g;
        b = t2 * (mColorTopTrans.b - mColorTopConst.b) + mColorTopConst.b;

        mColorTop = CL_Colorf(r, g, b);

        // Make middle of the sky darker (the night comes)
        r = t2 * (mColorMiddleTrans.r - mColorMiddleConst.r) + mColorMiddleConst.r;
        g = t2 * (mColorMiddleTrans.g - mColorMiddleConst.g) + mColorMiddleConst.g;
        b = t2 * (mColorMiddleTrans.b - mColorMiddleConst.b) + mColorMiddleConst.b;

        mColorMiddle = CL_Colorf(r, g, b);
    }

    if (mNight)
    {
        if (globalTime <= 21.5f) {
            t3 = 0.0f;
        } else if (globalTime >= 23.8f) {
            t3 = 1.0f;
            mNight = false;
        } else {
            t3 = (globalTime - 21.5f) / (23.8f - 21.5f);

            // After moon hiding the DAY becomes (make top part of the sky lighter)
            float r = t3 * (mColorTopConst.r - mColorTopTrans.r) + mColorTopTrans.r;
            float g = t3 * (mColorTopConst.g - mColorTopTrans.g) + mColorTopTrans.g;
            float b = t3 * (mColorTopConst.b - mColorTopTrans.b) + mColorTopTrans.b;

            mColorTop = CL_Colorf(r, g, b);

            // Make middle of the sky lighter (the day comes)
            r = t3 * (mColorMiddleConst.r - mColorMiddleTrans.r) + mColorMiddleTrans.r;
            g = t3 * (mColorMiddleConst.g - mColorMiddleTrans.g) + mColorMiddleTrans.g;
            b = t3 * (mColorMiddleConst.b - mColorMiddleTrans.b) + mColorMiddleTrans.b;

            mColorMiddle = CL_Colorf(r, g, b);
        }
    }

    CL_Draw::gradient_fill(mGC, quad1, CL_Gradient(mColorTop, mColorMiddle));
    CL_Draw::gradient_fill(mGC, quad2, CL_Gradient(mColorMiddle, mColorBottom));
}
