/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _BODY_VISUAL_HPP_
#define  _BODY_VISUAL_HPP_

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include "Physic/BodyState.hpp"

class BodyVisual
{
    private:
        BodyState *mBodyState;
    public:
        BodyVisual();
        void setBodyState(BodyState *state);

        void redrawBody();

        
};


#endif
