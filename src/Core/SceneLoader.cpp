/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

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

    TemplatesProcessor proc;
    std::string error= "";
    DocumentPtr document;
    try
    {
        document = proc.processTemplates("media/levels/" + sceneName);
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
    std::cout << "\t ---------- Parsing scene ----------\n";
    CL_DomElement world = root.get_elements_by_tag_name("World").item(0).to_element();
    CL_DomElement environ = world.get_elements_by_tag_name("Environ").item(0).to_element();
    CL_DomElement objects = world.get_elements_by_tag_name("Objects").item(0).to_element();

    // Get level attributes
    CL_String authors = root.get_attribute("authors");
    CL_String version = root.get_attribute("version");

    LOG(cl_format("Loading new scene with name `%1` for the game version: `%2`. Created by: `%3`."
                  , sceneName, version, authors));

    // Check versions of the game and the level
    if (boost::lexical_cast<float>(version.c_str()) != GAME_VERSION)
        LOG(cl_format("Warning! Level version is different with game one(%1). Possible faults.", version));

    // Get world dimensions
    int worldWidth = boost::lexical_cast<int>(world.get_attribute("size_width").c_str());
    int worldHeight = boost::lexical_cast<int>(world.get_attribute("size_height").c_str());
    LOG_NOFORMAT(cl_format("- World size width: %1\n- World size height: %2\n", worldWidth, worldHeight));

    // Check whether environ should be enabled
    bool mEnvironEnabled = false;
    if (environ.get_attribute("active") == "true") mEnvironEnabled = true;
    environManager().setEnvironEnabled(mEnvironEnabled);
    LOG_NOFORMAT(cl_format("- Environ enabled state: %1\n", mEnvironEnabled));

    // If Environ is enabled, go through all environ-params in the level file
    if (mEnvironEnabled)
    {
        environManager().initEnviron();

        std::map<std::string, EnvironTypes> deps;
        deps["Rain"] = Environ_Rain;
        deps["Clouds"] = Environ_Clouds;
        deps["Lightnings"] = Environ_Lightnings;
        deps["Sky"] = Environ_Sky;
        deps["Moon"] = Environ_Moon;
        deps["Leaves"] = Environ_Leaves;
        deps["Stars"] = Environ_Stars;
        deps["Birds"] = Environ_Birds;

        CL_DomNodeList childList = environ.get_child_nodes();
        for (int i=0; i < childList.get_length(); ++i)
        {
            CL_DomNode tag = childList.item(i);
            if (tag.get_node_name() == "Wind")
            {
                float pow = boost::lexical_cast<float>(tag.to_element().get_attribute("power").c_str());
                environManager().setWindPower(pow);
            }
            EnvironTypes type = deps[tag.get_node_name().c_str()];

            bool enabled = tag.to_element().get_attribute("enabled") == "true";

            int lim = -1;
            CL_String s_lim = tag.to_element().get_attribute("limit");
            if (s_lim != "") lim = boost::lexical_cast<int>(s_lim.c_str());

            environManager().enableType(enabled, type, lim);
        }
        LOG_NOFORMAT("- All environ objects are loaded.\n");
    }

    // START OF OBJECTS PARSING -------------------------------------------
    {

        CL_DomNodeList childList = objects.get_child_nodes();
        for (int i=0; i < childList.get_length(); ++i)
        {
            CL_DomElement tag = childList.item(i).to_element();

            std::string name = tag.get_attribute("name").c_str();
            std::string type = tag.get_attribute("type").c_str();
            LOG_NOFORMAT(cl_format("- Parsing object `%1` of type `%2`\n", name, type));

            // TODO: Call template parsing here...

            std::string desc = "";
            Object* object = typesManager().parseObject(&tag, type, desc);
            if(object != NULL)
            {
                objectsManager().addObject(object->getName(), object);
            }
            else
            {
                LOG(desc);
                LOG("Terminating parsing...");
                return;
            }
        }
    }
    // END OF OBJECTS PARSING -------------------------------------------

    //mMutex.unlock();
}
