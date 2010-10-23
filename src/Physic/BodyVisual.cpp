#include "Physic/BodyVisual.hpp"


BodyVisual::BodyVisual(Body *body)
{
    mBody = body;
    body->setVisual(this);
}

void BodyVisual::redrawBody()
{

}
