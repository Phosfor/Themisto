#ifndef _WEATHER_SKY_HPP_
#define _WEATHER_SKY_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class Sky
{
    private:
        CL_Colorf color1, color2, color3; // 1,2 - constans, 3 - changeable

        CL_Rectf quad1, quad2;

        int r1, g1, b1; // Used as const for lower sky-gradient
        int r2, g2, b2; // Atmosphere scattering

        float t1, t2;

        bool mNight;

        CL_GraphicContext mGC;

        float mWidth, mHeight;

    public:
        Sky(CL_Colorf &first = CL_Colorf::transparent,
                CL_Colorf &second = CL_Colorf::transparent);
        void update(float hours);
};

#endif /* _WEATHER_SKY_HPP_ */
