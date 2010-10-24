#include "World/WorldManager.hpp"

void WorldManager::initWorld()
{
    mDefaultMaterial = new BodyMaterial;
    mEnvironTemperature = 20;
}

WorldManager::~WorldManager()
{
    delete mDefaultMaterial;
}

