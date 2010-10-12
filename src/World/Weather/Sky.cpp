#include "World/Weather/Sky.hpp"

Sky::Sky()
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

    t1 = t2 = 0.0f;

    mGC = appManager.getGraphic();
    CL_Rectf geom = appManager.getWindow().get_geometry();
    mWidth = geom.get_width();
    mHeight = geom.get_height();

    quad1 = CL_Rectf(0, 0, mWidth, mHeight/2.0);
    quad2 = CL_Rectf(0, mHeight/2.0, mWidth, mHeight + mHeight * 0.3);
}

void Sky::update(float hours)
{
    // Make atmosphere scattering? [9; 12] hours
    if (hours <= 9.0f) {
        t1 = 0.0f;
    } else if (hours >= 11.0f) {
        t1 = 1.0f;
    } else {
        t1 = (hours - 9.0f) / (11.0f - 9.0f);

        // Make color transation of the sky bottom (to the scattering color)
        float r = t1 * (mColorBottomTrans.r - mColorBottomConst.r) + mColorBottomConst.r;
        float g = t1 * (mColorBottomTrans.g - mColorBottomConst.g) + mColorBottomConst.g;
        float b = t1 * (mColorBottomTrans.b - mColorBottomConst.b) + mColorBottomConst.b;

        mColorBottom = CL_Colorf(r, g, b);
    }

    if (hours <= 12.0f) {
        t2 = 0.0f;
    } else if (hours >= 14.0f) {
        t2 = 1.0f;
    } else {
        t2 = (hours - 12.0f) / (14.0f - 12.0f);

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

    CL_Draw::gradient_fill(mGC, quad1, CL_Gradient(mColorTop, mColorMiddle));
    CL_Draw::gradient_fill(mGC, quad2, CL_Gradient(mColorMiddle, mColorBottom));
}
