#include "World/Environ/EnvironObject.hpp"

void EnvironObject::setLimit(int limit)
{
   mMaxObjects = limit;
}

int EnvironObject::getLimit()
{
   return mMaxObjects;
}

void EnvironObject::setEnabled(bool state)
{
    mEnabled = state;
}

bool EnvironObject::getEnabled()
{
    return mEnabled;
}

EnvironObject::EnvironObject()
{
    mWindowWidth = ScreenResolutionX;
    mWindowHeight = ScreenResolutionY;
}
