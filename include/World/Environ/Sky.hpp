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

#ifndef _ENVIRON_SKY_HPP_
#define _ENVIRON_SKY_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"
#include "World/Environ/Object.hpp"

class Sky : public EnvironObject
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
        float t1, t2, t3;

        CL_GraphicContext mGC;

        bool mNight;

    public:
        Sky();
        void update(float elapsed);
};

#endif /* _ENVIRON_SKY_HPP_ */
