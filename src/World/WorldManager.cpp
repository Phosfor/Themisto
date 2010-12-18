/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/WorldManager.hpp"

void WorldManager::initWorld()
{

}

WorldManager::WorldManager()
{
    mDefaultMaterial = new BodyMaterial;
    mEnvironTemperature = 20;
    mUniqueIDCounter = 0;
}

WorldManager::~WorldManager()
{
    delete mDefaultMaterial;
}

std::string WorldManager::generateUniqueID()
{
    ++mUniqueIDCounter;
    std::ostringstream out;
    out << "#" << mUniqueIDCounter;
    return out.str(); 
}
