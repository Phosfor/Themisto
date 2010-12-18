/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Core/ResourceManager.hpp"

ResourceManager::ResourceManager()
{
    mediaPath = configManager().getValue<std::string>("application.media-folder", "media");
    fontsManager = CL_ResourceManager(mediaPath + "/fonts.xml");
    texturesManager = CL_ResourceManager(mediaPath + "/textures.xml");
}

void ResourceManager::loadTextures()
{
    using namespace std;

    LOG_NOFORMAT("\t---------- Loading textures ---------\n");
    unsigned int counter = 0;
    BOOST_FOREACH(CL_String section, texturesManager.get_resource_names())
    {
        CL_Resource textureSection = texturesManager.get_resource(section);
        string textureName = textureSection.get_element().get_attribute("name").c_str();
        string texturePath = textureSection.get_element().get_attribute("file").c_str();

        string sectionStd = string(section.c_str());
        unsigned int lastIndex = sectionStd.find_last_of('/');
        string formattedSection = sectionStd.substr(0, lastIndex);

        texturesStorage.insert(make_pair(formattedSection, make_pair(textureName, texturePath)));

        string fullPath = mediaPath + "/" + texturePath;
        if (!boost::filesystem::exists(fullPath))
        {
            LOG(cl_format("The texture path `%1` is invalid, check your resources!", fullPath));
        }
        counter++;
    }
    LOG(cl_format("%1 texture(s) are(is) loaded!", counter));
}

void ResourceManager::loadFonts()
{
    LOG_NOFORMAT("\t---------- Loading fonts ---------\n");
    BOOST_FOREACH(CL_String section, fontsManager.get_resource_names())
    {
        CL_Resource tempFontRes = fontsManager.get_resource(section);
        std::string filename = tempFontRes.get_element().get_attribute("file").c_str();

        std::string fontPath = mediaPath + "/" + filename;
        if (!boost::filesystem::exists(fontPath))
        {
            LOG(cl_format("The font path `%1` is invalid, check your resources!", fontPath));
        }
        else
        {
            std::vector<std::string> storage;
            boost::split(storage, section, boost::is_any_of("/"));
            std::string fontName = storage[storage.size()-1];
            CL_Font_System::register_font(fontPath, fontName);
            LOG(cl_format("Font with name `%1` is registered in the system.", fontName));
        }
    }
}
