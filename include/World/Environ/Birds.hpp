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

#ifndef _ENVIRON_BIRDS_HPP_
#define _ENVIRON_BIRDS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <boost/cstdint.hpp>

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
        uint16_t mProbability; // The probability that bird will begin to fly current frame
        bool mFirstTime;

        CL_GraphicContext mGC;

        void processBirds(CL_GraphicContext &gc, uint16_t width, uint16_t height, uint16_t i);

    public:
        explicit Birds(uint16_t maxBirds = 1);
        void setLimit(uint16_t limit);
        void update(float elapsed);
};

#endif /* _ENVIRON_BIRDS_HPP_ */
