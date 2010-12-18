/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
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
#include "World/Objects/Object.hpp"
#include "Core/ApplicationManager.hpp"

#include <map>
#include <utility>

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
