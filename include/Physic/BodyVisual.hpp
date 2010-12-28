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

#ifndef _BODY_VISUAL_HPP_
#define _BODY_VISUAL_HPP_

#include <Box2D/Collision/Shapes/b2PolygonShape.h>

#include "Physic/BodyState.hpp"
#include "Physic/Body.hpp"
#include "Core/Utils.hpp"

#include "Core/ResourceManager.hpp"
#include "Core/ApplicationManager.hpp"

class Body;
class BodyVisual
{
    private:
        BodyState *mBodyState;
        CL_Sprite mImageHandle;
        bool mHasVisual;
        Body *mParentBody;

    public:
        BodyVisual();
        void setBodyState(BodyState *state);
        bool getVisualState();
        void setVisualState(bool state);

        float mXPos, mYPos;
        float mSizeWidth, mSizeHeight;
        std::string mTextureName, mSectionName;

        void redrawBody();
        void configureVisual(Body *parent);
};

#endif /* _BODY_VISUAL_HPP_ */
