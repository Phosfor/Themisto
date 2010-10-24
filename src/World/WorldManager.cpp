#include "World/WorldManager.hpp"

void WorldManager::initWorld()
{

}

WorldManager::WorldManager()
{
    mDefaultMaterial = new BodyMaterial;
    mEnvironTemperature = 20;
}

WorldManager::~WorldManager()
{
    delete mDefaultMaterial;
}

