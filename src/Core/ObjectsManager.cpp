#include "Core/ObjectsManager.hpp"

ObjectsManager::ObjectsManager()
{
    mNumObjects = 0;
}

void ObjectsManager::update()
{
    for (ObjectMapType::const_iterator it=mObjects.begin();
            it != mObjects.end(); ++it)
    {
        /*TODO: Check whether object is out of screen */
        it->second->update();
    }
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
