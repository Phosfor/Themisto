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

#include "Core/SceneLoader.hpp"
#include "World/Objects/Object.hpp"

void SceneLoader::loadScene(const std::string &sceneName)
{
    //mThread.start<SceneLoader, const std::string&>(this, &SceneLoader::_threadWrapper, sceneName);
    //mThread.join();
    _threadWrapper(sceneName);
}


void SceneLoader::_threadWrapper(const std::string &sceneName)
{
    //mMutex.lock();
    std::cout << "\t ---------- Parsing scene ----------\n";

    TemplatesProcessor proc;
    std::string error= "";
    DocumentPtr document;
    try
    {
        document = proc.processTemplates(utils().getMediaFolder() + "/levels/" + sceneName);
        //CL_File writeFile("out.xml", CL_File::create_always, CL_File::access_write);
        //document->save(writeFile);
    }
    catch(CL_Exception& e)
    {
        LOG(e.what());
        LOG("Level parsing stopped.");
        return;
    }

    CL_DomElement root = document->get_document_element();
    CL_DomElement world = root.get_elements_by_tag_name("World").item(0).to_element();
    CL_DomElement environ = world.get_elements_by_tag_name("Environ").item(0).to_element();
    CL_DomElement objects = world.get_elements_by_tag_name("Objects").item(0).to_element();

    // Get level attributes
    CL_String authors = root.get_attribute("authors");
    CL_String version = root.get_attribute("version");
    CL_String name = root.get_attribute("name");

    LOG(cl_format("Loading new scene with name `%1` (in file `%2`) for the game version: `%3`. Created by: `%4`."
                  , name, sceneName, version, authors));

    // Check versions of the game and the level
    if (boost::lexical_cast<float>(version.c_str()) != GAME_VERSION)
        LOG(cl_format("Warning! Level version is different with game one(%1). Possible faults.", version));

    // Get world dimensions
    if (!world.has_attribute("image")) LOG_NOFORMAT("Error: World tag should have `image` attribute!");
    levelManager().init(world.get_attribute("image"));


    levelManager().setLevelName(name);

    // Camera viewport settings
    float cameraPosX = 0, cameraPosY = 0;
    if (world.has_attribute("cameraPosX"))
        cameraPosX = -abs(boost::lexical_cast<float>(world.get_attribute("cameraPosX").c_str()));

    if (world.has_attribute("cameraPosY"))
        cameraPosY = -abs(boost::lexical_cast<float>(world.get_attribute("cameraPosY").c_str()));

    CL_Rectf camViewport(CL_Pointf(cameraPosX, cameraPosY), CL_Sizef(ScreenResolutionX, ScreenResolutionY));
    levelManager().setCamViewport(camViewport);

    try
    {
        CL_DomNode foregroundNode = environ.select_node("Foreground");

        uint16_t actualSize = 0;
        if (foregroundNode.to_element().has_attribute("actual_size"))
        {
            actualSize = boost::lexical_cast<uint16_t>(foregroundNode.to_element().get_attribute("actual_size").c_str());
        }
        levelManager().setForegroundSize(actualSize);

        bool fixedImage = true;
        if (foregroundNode.to_element().has_attribute("fixed"))
        {
            fixedImage = foregroundNode.to_element().get_attribute("fixed") == "true";
        }
        levelManager().setForegroundFixed(fixedImage);
    }
    // Foreground tag isn't available
    catch (...) { }

    LOG_NOFORMAT("- Loading Environ objects\n");
    environManager().initEnviron();

    // CHANGEABLE ENVIRON [For grepping, don't remove]
    std::map<std::string, EnvironTypes> deps;
    deps["Rain"] = Environ_Rain;
    deps["Clouds"] = Environ_Clouds;
    deps["Foreground"] = Environ_Foreground;
    deps["Lightnings"] = Environ_Lightnings;
    deps["Sky"] = Environ_Sky;
    deps["Moon"] = Environ_Moon;
    deps["Leaves"] = Environ_Leaves;
    deps["Stars"] = Environ_Stars;
    deps["Birds"] = Environ_Birds;
    deps["Objects"] = Environ_Objects;

    CL_DomNodeList childList = environ.get_child_nodes();
    for (int i=0; i < childList.get_length(); ++i)
    {
        CL_DomNode tag = childList.item(i);
        if (tag.get_node_name() == "Wind")
        {
            float pow = boost::lexical_cast<float>(tag.to_element().get_attribute("power").c_str());
            environManager().setWindPower(pow);
        }
        if (tag.get_node_name() == "Foreground")
        {
            levelManager().setForegroundTexture(tag.to_element().get_attribute("image").c_str());
        }

        EnvironTypes type = deps[tag.get_node_name().c_str()];

        bool enabled = tag.to_element().get_attribute("enabled") == "true";

        float lim = -1;
        CL_String s_lim = tag.to_element().get_attribute("limit");
        if (s_lim != "") lim = boost::lexical_cast<float>(s_lim.c_str());

        if (enabled) environManager().enableType(enabled, type, lim);
    }
    LOG_NOFORMAT("- All environ objects are loaded.\n");

    // START OF OBJECTS PARSING -------------------------------------------
    {
        CL_DomNodeList childList = objects.get_child_nodes();
        for (int i=0; i < childList.get_length(); ++i)
        {
            CL_DomElement tag = childList.item(i).to_element();

            std::string name = tag.get_attribute("name").c_str();
            std::string type = tag.get_attribute("type").c_str();
            LOG_NOFORMAT(cl_format("- Parsing object `%1` of type `%2`\n", name, type));

            int z_index = 1;
            if (tag.has_attribute("z-index"))
                z_index = boost::lexical_cast<int>(tag.get_attribute("z-index").c_str());

            bool always_draw = false;
            if (tag.has_attribute("always_draw"))
                always_draw = tag.get_attribute("always_draw") == "true";

            std::string desc = "";
            boost::shared_ptr<Object> object;
            try
            {
                object = typesManager().parseObject(&tag, type, desc);
            }
            catch(CL_Exception& e)
            {
                LOG(e.what());
                LOG("Level parsing stopped.");
                return;
            }
            object->setIndex(z_index);
            object->setType(type);
            object->setAlwaysDraw(always_draw);
            levelManager().addObject(name, object);
        }
    }
    // END OF OBJECTS PARSING -------------------------------------------

    //mMutex.unlock();
}
