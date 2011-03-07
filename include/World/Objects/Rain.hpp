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

#pragma once

#include <math.h>

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <boost/cstdlib.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"
#include "World/Objects/Object.hpp"

const float kTail = 0.025f;
struct Data
{
    float x, y, x_speed, y_speed, timeout;
    Data();
};

class Rain : public Object
{
    private:
        std::vector<Data> mData;
        CL_Colorf mDropColor;
        CL_GraphicContext mGC;
        float mKoef1;

        int mMaxObjects;
        float mWindowHeight, mWindowWidth;

        void processDrops(float windPower, Data &current, bool firstTime=false);

    public:
        explicit Rain(uint16_t maxDrops);

        static boost::shared_ptr<Object> ParseRain(CL_DomElement* element, std::string& desc);

        // --- Object implementation ---
        bool checkCollision(CL_Pointf point);
        void init();
        void update(float elapsed);
        void updateVisual(float newX, float newY);

        CL_Pointf getPosition();
        void setPosition(CL_Pointf newPos);

        CL_Rectf getRectangle();
};
