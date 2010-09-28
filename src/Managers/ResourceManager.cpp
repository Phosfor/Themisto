#include "Managers/ResourceManager.hpp"

ResourceManager::ResourceManager()
{
    fontsManagers = CL_ResourceManager(utils.getMediaFolder() + "/fonts.xml");
    texturesManagers = CL_ResourceManager(utils.getMediaFolder() + "/textures.xml");
}

ResourceManager::loadTextures()
{

}

ResourceManager::loadFonts()
{

}
