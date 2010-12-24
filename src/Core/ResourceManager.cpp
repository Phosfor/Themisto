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

    textureXmlFile = CL_File(mediaPath + "/textures.xml");
    textureXmlDocument = CL_DomDocument(textureXmlFile);
    textureXmlRoot = textureXmlDocument.get_document_element();
}

CL_Image ResourceManager::getImage(const std::string &section, const std::string &name)
{
    if (mTexturesData.find(section + '/' + name) == mTexturesData.end())
        throw new CL_Exception(cl_format("Failed to getImage from `%1/%2`.", section, name));

    CL_Texture temp = mTexturesData[section + '/' + name];
    return CL_Image(appManager().getGraphic(), temp, CL_Rectf(0, 0, temp.get_width(), temp.get_height()));
}

CL_Sprite ResourceManager::getSprite(const std::string &section, const std::vector<std::string> &names)
{
    CL_SpriteDescription desc;

    BOOST_FOREACH(std::string name, names)
    {
        if (mTexturesData.find(section + '/' + name) == mTexturesData.end())
            throw new CL_Exception(cl_format("Failed to getSprite from `%1/%2`.", section, name));

        CL_Texture temp = mTexturesData[section + '/' + name];
        desc.add_frame(temp);
    }

    return CL_Sprite(appManager().getGraphic(), desc);
}

CL_Sprite ResourceManager::getSprite(const std::string &section, const std::string &name)
{
    if (mTexturesData.find(section + '/' + name) == mTexturesData.end())
        throw new CL_Exception(cl_format("Failed to getSprite from `%1/%2`.", section, name));

    CL_Texture temp = mTexturesData[section + '/' + name];
    CL_SpriteDescription desc;
    desc.add_frame(temp);

    return CL_Sprite(appManager().getGraphic(), desc);
}

void ResourceManager::loadTextures()
{
    using namespace std;

    LOG_NOFORMAT("\t---------- Loading textures ---------\n");
    unsigned int counter = 0;
    BOOST_FOREACH(CL_String section, texturesManager.get_section_names())
    {
        LOG(cl_format("Parsing section `%1`", section.c_str()));

        BOOST_FOREACH(CL_String resource, texturesManager.get_resource_names(section))
        {
            CL_Resource textureSection = texturesManager.get_resource(section + resource);
            CL_String textureName = textureSection.get_element().get_attribute("name");
            CL_String texturePath = textureSection.get_element().get_attribute("file");

            texturesStorage[section][textureName] = texturePath;
            std::string res = std::string(cl_format("%1%2", section, textureName).c_str());
            mTexturesData[res] = CL_Texture(appManager().getGraphic(), cl_format("%1/%2", mediaPath, texturePath));

            CL_String fullPath = cl_format("%1/%2", mediaPath, texturePath);
            if (!boost::filesystem::exists(fullPath.c_str()))
            {
                LOG(cl_format("The texture path `%1` is invalid, check your resources!", fullPath));
            }
            counter++;
        }
    }
    LOG(cl_format("%1 texture(s) are(is) loaded!", counter));
}

CL_DomNode ResourceManager::sectionHandle(const std::string &section)
{
    try {
        BOOST_FOREACH(CL_DomNode node, textureXmlRoot.select_nodes("section"))
        {
            if (node.to_element().get_attribute("name").c_str() == section)
                return node;
        }
    } catch(CL_Exception &e) {
        std::cout << "Something bad happened in `sectionHandle`: " << e.what();
    }

    return CL_DomNode();
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
