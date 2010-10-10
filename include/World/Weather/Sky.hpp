#ifndef _WEATHER_SKY_HPP_
#define _WEATHER_SKY_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class Sky
{
    private:
        CL_Colorf color1;
        CL_Colorf color2;

        int r1, g1, b1; // Used as const for lower sky-gradient
        int r2, g2, b2; // Atmosphere scattering
        float t;

        bool mNight;

        CL_GraphicContext mGC;
        CL_Rect mGeom;

    public:
        Sky(CL_Colorf &first = CL_Colorf::transparent,
                CL_Colorf &second = CL_Colorf::transparent);
        void update(float hours);
};

#endif /* _WEATHER_SKY_HPP_ */
