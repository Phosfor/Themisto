/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Core/ObjectsManager.hpp"

ObjectsManager::ObjectsManager()
{
    mNumObjects = 0;
    mAppManager = &appManager;
}

void ObjectsManager::update()
{
    for (ObjectMapType::const_iterator it=mObjects.begin();
            it != mObjects.end(); ++it)
    {
        /*TODO: Check whether object is out of screen */
        it->second->update(mAppManager->getElapsed());
    }
}

void ObjectsManager::addObject(const std::string &name, Object *obj)
{
    if (mObjects.find(name) == mObjects.end())
    {
        mObjects.insert(make_pair(name, obj));
        mNumObjects++;
    }
    else
    {
        LOG(cl_format("Object with name `%1` already exists!", name));
    }
}
