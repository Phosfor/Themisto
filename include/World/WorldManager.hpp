/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _WORLD_MANAGER_HPP_
#define _WORLD_MANAGER_HPP_

#include "Physic/BodyMaterial.hpp"

#include <boost/serialization/singleton.hpp>
#include <sstream>

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
        std::string generateUniqueID();
};

inline WorldManager &worldManager() { return WorldManager::get_mutable_instance(); }

#endif /* _WORLD_MANAGER_HPP_ */
