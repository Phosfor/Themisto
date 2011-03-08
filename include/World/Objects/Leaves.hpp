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

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <boost/cstdlib.hpp>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/WorldManager.hpp"
#include "World/Objects/Object.hpp"

struct LeafData
{
    float x, y, x_speed, y_speed, k1, k2, speed_koef;
    uint16_t leafType, timeout;

    CL_Sprite imageHandle;

    bool animation;
    float addedAngle, timer;
};

class Leaves : public Object
{
    private:
        std::vector<LeafData> mLeaves;
        bool mFirstTime;
        float functionValue;
        uint16_t mMaxObjects;

        CL_GraphicContext mGC;

        void processLeaves(CL_GraphicContext &gc, float windPower, LeafData &current, bool firstTime = false);

    public:
        explicit Leaves(uint16_t maxLeaves = 5);

        static boost::shared_ptr<Object> ParseLeaves(CL_DomElement* tag, std::string &desc);

        // --- Object implementation ---
        bool checkCollision(CL_Pointf point);
        CL_Pointf getPosition();
        void setPosition(CL_Pointf newPos);
        CL_Rectf getRectangle();

        void init();
        void update(float elapsed);
        void updateVisual(float newX, float newY);
};
