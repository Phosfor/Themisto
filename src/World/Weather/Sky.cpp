#include "World/Weather/Sky.hpp"

Sky::Sky(CL_Colorf &first, CL_Colorf &second)
{
    if (first == CL_Colorf::transparent || second == CL_Colorf::transparent)
    {
        r1 = 83;
        g1 = 178;
        b1 = 234;
        color1 = CL_Colorf(25/255.0f, 60/255.0f, 141/255.0f);
        color2 = CL_Colorf(54/255.0f, 119/255.0f, 198/255.0f);
        color3 = CL_Colorf(83/255.0f, 178/255.0f, 234/255.0f);
    }
    else
    {
        r1 = second.r;
        g1 = second.g;
        b1 = second.b;
        color1 = first;
        color2 = second;
    }

    // Scattering color
    r2 = 234;
    g2 = 197;
    b2 = 83;

    mNight = false;
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
        float r = t1 * (r2 - r1) + r1;
        float g = t1 * (g2 - g1) + g1;
        float b = t1 * (b2 - b1) + b1;

        color3 = CL_Colorf(r/255.0f, g/255.0f, b/255.0f);
    }

    if (hours <= 12.0f) {
        t2 = 0.0f;
    } else if (hours >= 14.0f) {
        t2 = 1.0f;
    } else {
        t2 = (hours - 12.0f) / (14.0f - 12.0f);
        float r = t2 * (r1 - r2) + r2;
        float g = t2 * (g1 - g2) + g2;
        float b = t2 * (b1 - b2) + b2;

        color3 = CL_Colorf(r/255.0f, g/255.0f, b/255.0f);
    }

    CL_Draw::gradient_fill(mGC, quad1, CL_Gradient(color1, color2));
    CL_Draw::gradient_fill(mGC, quad2, CL_Gradient(color2, color3));
}
