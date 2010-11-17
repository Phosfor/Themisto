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

string WorldManager::generateUniqueID()
{
    ++mUniqueIDCounter;
    std::ostringstream out;
    out << "#" << mUniqueIDCounter;
    return out.str(); 
}
