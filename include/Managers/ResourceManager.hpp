#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <boost/serialization/singleton.hpp>

#include "Core/Utils.hpp"

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
