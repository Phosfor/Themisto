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

#ifndef _CORE_OBJECTS_MANAGER_HPP_
#define _CORE_OBJECTS_MANAGER_HPP_

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include <boost/serialization/singleton.hpp>
#include "Core/ConfigManager.hpp"
#include "Core/LogManager.hpp"
#include "Core/ApplicationManager.hpp"

#include <map>
#include <utility>

class Object;
typedef std::map<std::string, Object*> ObjectMapType;
class ObjectsManager : public boost::serialization::singleton<ObjectsManager>
{
    private:
        ObjectMapType mObjects;
        unsigned int mNumObjects;

    public:
        ObjectsManager();
        void addObject(const std::string &name, Object *obj);
        void update();
};

inline ObjectsManager &objectsManager() { return ObjectsManager::get_mutable_instance(); }

#endif /* _CORE_OBJECTS_MANAGER_HPP_ */
