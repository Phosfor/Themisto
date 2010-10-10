#include "World/Weather/Sky.hpp"

Sky::Sky(CL_Colorf &first, CL_Colorf &second)
{
    if (first == CL_Colorf::transparent || second == CL_Colorf::transparent)
    {
        color1 = CL_Colorf(25/255.0f, 60/255.0f, 141/255.0f);
        color2 = CL_Colorf(83/255.0f, 178/255.0f, 234/255.0f);
    }
    else
    {
        color1 = first;
        color2 = second;
    }

    mNight = false;
    mAtmosphereScattering = false;

    mGC = appManager.getGraphic();
    mGeom = appManager.getWindow().get_geometry();
}

void Sky::update()
{
    CL_Draw::gradient_fill(mGC, CL_Rectf(0, 0, mGeom.get_width(), mGeom.get_height()), 
            CL_Gradient(color1, color2));
}
