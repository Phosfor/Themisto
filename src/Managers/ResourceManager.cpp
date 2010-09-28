#include "Managers/ResourceManager.hpp"

ResourceManager::ResourceManager()
{
    string mediaPath = configManager.getValue<string>("application.media-folder", "media");
    fontsManager = CL_ResourceManager(mediaPath + "/fonts.xml");
    texturesManager = CL_ResourceManager(mediaPath + "/textures.xml");
}

void ResourceManager::loadTextures()
{

}

void ResourceManager::loadFonts()
{
    BOOST_FOREACH(CL_String section, fontsManager.get_resource_names())
    { 
        CL_Resource tempFontRes = fontsManager.get_resource(section);
        CL_String filename = tempFontRes.get_element().get_attribute("file");

        string mediaPath = configManager.getValue<string>("application.media-folder", "media");
        string fontPath = mediaPath + "/" + filename.c_str();
        if (!boost::filesystem::exists(fontPath))
        {
            LOG(FORMAT("The font path `%1%` is invalid, check your resources!", fontPath));
        }
        else
        {
            std::vector<std::string> storage;
            boost::split(storage, section, boost::is_any_of("/"));
            string fontName = storage[storage.size()-1];
            CL_Font_System::register_font(fontPath, fontName);
            LOG(FORMAT("Font with name `%1%` is registered in the system.", fontName));
        }
    }
}
