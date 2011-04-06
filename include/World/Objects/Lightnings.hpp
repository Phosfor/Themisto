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
#include "World/Objects/Object.hpp"
#include "Core/LevelManager.hpp"


class Clouds;
class Lightnings : public Object
{
    private:
        Clouds *mCloudsHandle;

        CL_Pointf mPosition;
        std::vector<CL_Image> mLightningImages;

        const uint16_t mProbability;
        bool mAnimation;
        float mTimer, alpha;
        uint16_t mHandle;
        float mMaxObjects;
        float mWindowHeight, mWindowWidth;
        bool mEnabled;


        CL_GraphicContext mGC;

    public:
        Lightnings();

        bool checkCollision(CL_Pointf point);
        void setPosition(CL_Pointf newPos);
        CL_Pointf getPosition();
        CL_Rectf getRectangle();

        void update(float elapsed);
        void updateVisual(float newX, float newY);

        static boost::shared_ptr<Object> ParseLightnings(CL_DomElement element);
};
