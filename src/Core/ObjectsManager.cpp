/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
        it->second->update(appManager().getElapsed());
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
