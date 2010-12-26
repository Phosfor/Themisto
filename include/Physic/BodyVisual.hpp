/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
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

    public:
        BodyVisual();
        void setBodyState(BodyState *state);
        bool getVisualState();
        void setVisualState(bool state);

        float mXPos, mYPos;
        float mSizeWidth, mSizeHeight;
        std::string mTextureName, mSectionName;

        void redrawBody(Body &body);
        void configureVisual();
};

#endif /* _BODY_VISUAL_HPP_ */
