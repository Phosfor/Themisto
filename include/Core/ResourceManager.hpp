/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/ConfigManager.hpp"
#include "Core/LogManager.hpp"
#include "Core/Utils.hpp"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/serialization/singleton.hpp>

#include <vector>

class ResourceManager : public boost::serialization::singleton<ResourceManager>
{
    private:
        CL_ResourceManager fontsManager, texturesManager;

    public:
        ResourceManager();

        void loadFonts();
        void loadTextures();
};

inline ResourceManager &resourceManager() { return ResourceManager::get_mutable_instance(); }

#endif /* _RESOURCE_MANAGER_HPP_ */
