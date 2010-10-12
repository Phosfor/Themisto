#ifndef _WEATHER_SKY_HPP_
#define _WEATHER_SKY_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class Sky
{
    private:
        CL_Rectf quad1, quad2; // The sky is splitted into 2 quads

        // Current colors for the gradients (are changeable)
        CL_Colorf 
            mColorTop,
            mColorMiddle,
            mColorBottom;

        // Constant colors, used to get back old colors after transactions
        CL_Colorf 
            mColorTopConst,
            mColorMiddleConst,
            mColorBottomConst;

        // Transation colors
        CL_Colorf
            mColorTopTrans,     // After moon hiding color (night)
            mColorMiddleTrans,  // After moon hiding color (night)
            mColorBottomTrans;  // Scattering color

        // Used as koeficient [0; 1] with color transations
        float t1, t2;

        CL_GraphicContext mGC;
        float mWidth, mHeight;

    public:
        Sky();
        void update(float hours);
};

#endif /* _WEATHER_SKY_HPP_ */
