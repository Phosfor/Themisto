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

#define resourceManager (ResourceManager::get_mutable_instance())
#define resourceManagerConst (ResourceManager::get_const_instance())

class ResourceManager : public boost::serialization::singleton<ResourceManager>
{
    private:
        CL_ResourceManager fontsManager, texturesManager;

    public:
        ResourceManager();

        void loadFonts();
        void loadTextures();
};


#endif /* _RESOURCE_MANAGER_HPP_ */
