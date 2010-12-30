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

#ifndef _ENVIRON_BIRDS_HPP_
#define _ENVIRON_BIRDS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "World/Environ/Object.hpp"

struct BirdData
{
    float x, y, x_speed, scale, side, timeout;
    CL_SpriteDescription mImageDesc;
    CL_Sprite mBirdImage;
};

class Birds : public EnvironObject
{
    private:
        std::vector<BirdData> mBirds;
        int mProbability; // The probability that bird will begin to fly current frame
        bool mFirstTime;

        CL_GraphicContext mGC;

        void processBirds(CL_GraphicContext &gc, int width, int height, int i);

    public:
        Birds(int maxBirds = 1);
        void setLimit(int limit);
        void update(float windPower, float elapsed, float globalTime);
};

#endif /* _ENVIRON_BIRDS_HPP_ */
