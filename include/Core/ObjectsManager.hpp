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

#include <map>
#include <utility>

#define objectsManager (ObjectsManager::get_mutable_instance())
#define objectsManagerConst (ObjectsManager::get_const_instance())

class ObjectsManager : public boost::serialization::singleton<ObjectsManager>
{
    private:
        std::map<std::string, Object*> mObjects;
        unsigned int mNumObjects;

    public:
        ObjectsManager();
        void addObject(const std::string &name, Object *obj);
        void update();
};

#endif /* _CORE_OBJECTS_MANAGER_HPP_ */
