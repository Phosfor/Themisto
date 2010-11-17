#ifndef _WORLD_MANAGER_HPP_
#define _WORLD_MANAGER_HPP_

#include "Physic/BodyMaterial.hpp"

#include <boost/serialization/singleton.hpp>
#include <sstream>

#define worldManager (WorldManager::get_mutable_instance())
#define worldManagerConst (WorldManager::get_const_instance())

class WorldManager : public boost::serialization::singleton<WorldManager>
{
    private:
        int mUniqueIDCounter;

    public:
        BodyMaterial *mDefaultMaterial; // Sharing default material for memory economy reason
        float mEnvironTemperature;

        void initWorld();
        WorldManager();
        ~WorldManager();
        string generateUniqueID();
};

#endif /* _WORLD_MANAGER_HPP_ */
