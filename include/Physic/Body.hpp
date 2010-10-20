#ifndef _PHYSIC_BODY_HPP_
#define _PHYSIC_BODY_HPP_

#include <Box2D/Box2D.h>

class Body
{
    private:
        b2Body* mBody;

    public:
        Body();
        Body(b2Body* body);
        b2Body& getb2Body();
        void updateVisual();
};

#endif
