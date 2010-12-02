/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Environ/Object.hpp"

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
