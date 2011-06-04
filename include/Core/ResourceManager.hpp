/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <vector>
#include <string>
#include <map>
#include <utility>

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/serialization/singleton.hpp>

#include "Core/ApplicationManager.hpp"
#include "Core/ConfigManager.hpp"
#include "Core/LogManager.hpp"
#include "Core/ScriptsManager.hpp"
#include "Core/GuiManager.hpp"
#include "Core/Utils.hpp"

class ResourceManager : public boost::serialization::singleton<ResourceManager>
{
    private:
        CL_ResourceManager fontsManager, texturesManager;
        std::string mediaPath;

        CL_File textureXmlFile;
        CL_DomDocument textureXmlDocument;
        CL_DomElement textureXmlRoot;

        // Section => (name => location)
        std::map<CL_String, std::map<CL_String, CL_String>> texturesStorage;
        std::map<CL_String, CL_Texture> mTexturesData; // Stores copy of the texture in memory

    public:
        ResourceManager();

        void loadFonts();
        void loadTextures();

        std::string getImagePath(const std::string &section, const std::string &name);
        CL_Image getImage(const std::string &section, const std::string &name);
        CL_Sprite getSprite(const std::string &section, boost::python::object names);
        //CL_Sprite getSprite(const std::string &section, const std::vector<std::string> &names);
        CL_Sprite getSprite(const std::string &section, const std::string &name);
        CL_DomNode sectionHandle(const std::string &section);
};

inline ResourceManager &resourceManager() { return ResourceManager::get_mutable_instance(); }
