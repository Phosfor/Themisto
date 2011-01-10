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

#ifndef _ENVIRON_CLOUDS_HPP_
#define _ENVIRON_CLOUDS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/EnvironManager.hpp"
#include "World/Environ/Object.hpp"
#include "Core/LevelManager.hpp"

struct CloudData
{
        float x, y_offset, x_speed, speed_koef;
        uint16_t cloudType, timeout;
        CL_Sprite imageHandle;
        CL_Colorf mColor;
};

class Clouds : public EnvironObject
{
    private:
        std::vector<CloudData> mClouds;
        CL_GraphicContext mGC;
        bool mFirstTime;

        void processClouds(CL_GraphicContext &gc, float windPower, CloudData &current, bool firstTime=false);

    public:
        explicit Clouds(uint16_t maxClouds = 20);
        void setLimit(uint16_t limit);
        void update(float elapsed);

        // For the lightning
        CL_Pointf getCloudPos();
};

#endif /* _ENVIRON_CLOUDS_HPP_ */
