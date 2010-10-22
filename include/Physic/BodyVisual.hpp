#ifndef _BODY_VISUAL_HPP_
#define  _BODY_VISUAL_HPP_

#include "Core/Application.hpp"
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include "Physic/Body.hpp"

class Body;



class BodyVisual
{
    private:
        Body *mBody;

    public:
        BodyVisual(Body *body);
        void redrawBody();
};

#endif
