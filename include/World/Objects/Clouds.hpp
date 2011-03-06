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

#ifndef _CLOUDS_OBJECT_HPP_
#define _CLOUDS_OBJECT_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/EnvironManager.hpp"
#include "World/Objects/Object.hpp"
#include "Core/LevelManager.hpp"

struct CloudData
{
        float x, y_offset, x_speed, speed_koef;
        uint16_t cloudType, timeout;
        CL_Sprite imageHandle;
        CL_Colorf mColor;
};

class Clouds : public Object
{
    private:
        std::vector<CloudData> mClouds;
        CL_GraphicContext mGC;
        bool mFirstTime;
        float mMaxObjects;
        int mWindowHeight, mWindowWidth;
        uint16_t mActualSize;

        void processClouds(CL_GraphicContext &gc, float windPower, CloudData &current, bool firstTime=false);

    public:
        explicit Clouds(uint16_t maxClouds);
        void setLimit(uint16_t limit);

        void setPosition(CL_Pointf newPos);
        CL_Pointf getPosition();
        CL_Rectf getRectangle();

        void init();
        void update(float elapsed);
        void updateVisual(float newX, float newY);

        static boost::shared_ptr<Object> ParseClouds(CL_DomElement* cloudsElement, std::string& desc);

        // For the lightning
        CL_Pointf getCloudPos();
};

#endif /* _CLOUDS_OBJECT_HPP_ */
