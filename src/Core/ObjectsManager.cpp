#include "Core/ObjectsManager.hpp"

ObjectsManager::ObjectsManager()
{
    mNumObjects = 0;
}

void ObjectsManager::update()
{
    // ...
}

void ObjectsManager::addObject(const std::string &name, Object *obj)
{
    if (mObjects.find(name) != mObjects.end())
    {
        mObjects.insert(make_pair(name, obj));
        mNumObjects++;
    }
    else
    {
        LOG(cl_format("Object with name `%1` already exists!", name));
    }
}
