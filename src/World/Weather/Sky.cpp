#include "World/Weather/Sky.hpp"

Sky::Sky(CL_Colorf &first, CL_Colorf &second)
{
    if (first == CL_Colorf::transparent || second == CL_Colorf::transparent)
    {
        r1 = 83;
        g1 = 178;
        b1 = 234;
        color1 = CL_Colorf(25/255.0f, 60/255.0f, 141/255.0f);
        color2 = CL_Colorf(83/255.0f, 178/255.0f, 234/255.0f);
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
    //colorScattering = color2;

    mNight = false;
    t = 0.0f;

    mGC = appManager.getGraphic();
    mGeom = appManager.getWindow().get_geometry();

    pos1 = CL_Pointf(0.0, 0.0);
    pos2 = CL_Pointf(mGeom.get_width(), mGeom.get_height());
}

void Sky::update(float hours)
{
    // Make atmosphere scattering? [9; 12] hours
    if (hours <= 9.0f)
    {
        t = 0.0f;
    }
    else if (hours >= 12.0f)
    {
        t = 1.0f;
    }
    else
    {
        t = (hours - 9.0f) / (12.0f - 9.0f);
        float r = t * (r2 - r1) + r1;
        float g = t * (g2 - g1) + g1;
        float b = t * (b2 - b1) + b1;

        pos2 += t * 10.0f;

        color2 = CL_Colorf(r/255.0f, g/255.0f, b/255.0f);
    }

    CL_Draw::gradient_fill(mGC,
            pos1, pos2,
            CL_Gradient(color1, color2));
    /*CL_Draw::gradient_fill(mGC, CL_Pointf(0.0, 0.0),
            CL_Pointf(mGeom.get_width()*1.3, mGeom.get_height()*1.3),
            CL_Gradient(CL_Colorf::transparent, colorScattering));*/
}
