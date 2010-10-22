#include "World/Environ/EnvironObject.hpp"

void EnvironObject::setLimit(int limit)
{
   mMaxObjects = limit;
}

int EnvironObject::getLimit()
{
   return mMaxObjects;
}

EnvironObject::EnvironObject()
{
    mWindowWidth = ScreenResolutionX;
    mWindowHeight = ScreenResolutionY;
}
