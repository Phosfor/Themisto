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

#include <map>
#include <utility>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include <boost/serialization/singleton.hpp>
#include "Core/ConfigManager.hpp"
#include "Core/LogManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Utils.hpp"

struct Compare
{
    bool operator()(const std::string &a, const std::string &b) const
    {
        unsigned int index1 = a.find_first_of('_');
        std::string num1 = a.substr(0, index1);
        int num_1 = atoi(num1.c_str());

        unsigned int index2 = b.find_first_of('_');
        std::string num2 = b.substr(0, index2);
        int num_2 = atoi(num2.c_str());

        return num_1 <= num_2;
    }
};

class Object;
typedef std::map<std::string, Object*> ObjectMapTypeAccess;
typedef std::map<std::string, Object*, Compare> ObjectMapTypeSorted;
class ObjectsManager : public boost::serialization::singleton<ObjectsManager>
{
    private:
        ObjectMapTypeAccess mObjects;
        ObjectMapTypeSorted mObjectsSorted;
        unsigned int mNumObjects;

    public:
        ObjectsManager();
        void addObject(const std::string &name, Object *obj);
        void update();
};

inline ObjectsManager &objectsManager() { return ObjectsManager::get_mutable_instance(); }

#endif /* _CORE_OBJECTS_MANAGER_HPP_ */
