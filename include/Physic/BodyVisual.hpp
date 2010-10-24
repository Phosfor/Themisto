#ifndef _BODY_VISUAL_HPP_
#define  _BODY_VISUAL_HPP_

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include "Physic/BodyState.hpp"

class BodyVisual
{
    public:
        BodyVisual();
        void setBodyState(BodyState *state);
        void redrawBody();

        BodyState *mBodyState;
};


#endif
