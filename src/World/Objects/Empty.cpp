#include "World/Objects/Empty.hpp"

EmptyObject::EmptyObject()
{
    this->setType("Empty");
}
void EmptyObject::updateVisual(float newX, float newY)
{
    LOG("You shoudln't update empty object!");
}

void EmptyObject::update(float elapsed)
{
    LOG("You shoudln't update empty object!");
}

CL_Pointf EmptyObject::getPosition() { return CL_Pointf(); }
void EmptyObject::setPosition(CL_Pointf newPos) { }
CL_Rectf EmptyObject::getRectangle() { return CL_Rectf(); }
bool EmptyObject::checkCollision(CL_Pointf point) { return false; }
