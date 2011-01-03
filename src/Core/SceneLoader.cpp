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

void SceneLoader::copyAttributes(CL_DomNode parent, CL_DomNode templatePointer)
{
    // Go through all attributes from template and copy them
    CL_DomNamedNodeMap nodemap = templatePointer.get_attributes();
    for (int i=0; i < nodemap.get_length(); ++i) 
    {
        CL_DomAttr attribute = nodemap.item(i).to_attr();
        parent.to_element().set_attribute(attribute.get_name(), attribute.get_value());
    }
}

void SceneLoader::processTags(CL_DomNode parent, CL_DomNode templatePointer, CL_DomDocument document)
{
    std::cout << "ProcessTags iteration...\n";
    std::cout << "parent node_name: " << parent.get_node_name().c_str() << "\n";
    std::cout << "template node_name: " << templatePointer.get_node_name().c_str() << "\n";

    // Tags are equivalent
    if ( (parent.get_node_name() == templatePointer.get_node_name()) &&
         (parent.to_element().get_attribute("id") == templatePointer.to_element().get_attribute("id")) )
    {
        std::cout << "Tags are equivalent!\n";

        std::cout << "Copy attributes!\n";
        copyAttributes(parent, templatePointer);

        CL_String action = "merge";
        if (parent.to_element().has_attribute("action")) action = parent.to_element().get_attribute("action");

        std::cout << "parent action: " << action.c_str() << "\n";

        // If ignore - remove element from xml-tree
        if (action == "ignore")
        {
            document.remove_child(parent);
        }
        else if (action == "override")
        {
            std::cout << "Override action, returning...\n";
            return;
        }
        else if (action == "merge")
        {
            std::cout << "Process merge action...\n";
            CL_DomNodeList parentChildren = parent.get_child_nodes();
            for (int i=0; i < parentChildren.get_length(); ++i)
            {
                std::cout << "\t - Iteration through parent child...\n";
                bool processNodes = true;
                CL_DomNode searchNode;

                std::cout << "\t - Searching node with name: " << parentChildren.item(i).get_node_name().c_str() << "\n";
                try {
                    // Try to find such node in template
                    searchNode = templatePointer.select_node(parentChildren.item(i).get_node_name());
                    if (searchNode.to_element().get_attribute("id") != parentChildren.item(i).to_element().get_attribute("id"))
                        processNodes = false;
                } catch(...) { processNodes = false; }

                if (processNodes)
                {
                    std::cout << "Found equivalent child nodes!\n";
                    processTags(parentChildren.item(i), searchNode, document);
                    templatePointer.remove_child(searchNode);
                }
                else
                {
                    std::cout << "\t - Didn't find such node name in template!\n";
                }
            }

            CL_DomNodeList anotherChildren = templatePointer.get_child_nodes();
            std::cout << "Merging another children counting: " << anotherChildren.get_length() << " from the template...\n";
            for (int i=0; i < anotherChildren.get_length(); ++i)
            {
                std::cout << "Appending child with name: " << anotherChildren.item(i).get_node_name().c_str() << "\n";
                parent.append_child(document.import_node(anotherChildren.item(i), true));
            }
        }
    }
}

void SceneLoader::processTemplates(CL_DomDocument document)
{
    CL_DomNodeList childList = document.get_child_nodes();

    for (int i=0; i < childList.get_length(); ++i)
    {
        CL_DomElement parent = childList.item(i).to_element();

        // If current tag doesn't have template argument, skip this iteration
        if (!parent.to_element().has_attribute("template")) continue;

        std::string tplLocation = parent.to_element().get_attribute("template").c_str();
        std::string parentNodeName = parent.get_node_name().c_str();

        // template="Filename.tpl/Tplname"
        unsigned int dotPos = tplLocation.find(".");
        if (dotPos == std::string::npos) 
            std::cout << cl_format("Wrong template location `%1`\n", tplLocation).c_str();

        std::string tplFile = tplLocation.substr(0, dotPos+4);
        std::string tplName = tplLocation.substr(dotPos+5, tplLocation.length());

        if (!boost::filesystem::exists("media/templates/" + tplFile))
            LOG(cl_format("Please check template name, it's incorrect: `%1`\n", tplFile).c_str());

        /*TODO: Check tplFile exists with boost::filesystem */
        // Template file stuff
        CL_File tplFileHandle("templates/" + tplFile);
        CL_DomDocument tplDocumentHandle(tplFileHandle);

        CL_DomElement templatePointer; // This will store element with need template name
        CL_DomNodeList tplListHandle = tplDocumentHandle.get_child_nodes();

        // Go through template tags and find one with need template name
        for (int j=0; j < tplListHandle.get_length(); ++j)
        {
            CL_DomElement tagElem = tplListHandle.item(j).to_element();
            if (tagElem.get_node_name() != "Template") continue;

            // If current tag's name is equal for given template's name.
            if (tagElem.get_attribute("name").c_str() == tplName)
            {
                templatePointer = tagElem;
                break;
            }
        }

        // If we didn't find Template tag with need name
        if (templatePointer.is_null())
            std::cout << cl_format("Failed to find template `%1` in file `%2`\n", tplName, tplFile).c_str();

        // Now we have CL_DomElement object which contains some Template-tag
        if (templatePointer.get_child_nodes().get_length() > 1)
            std::cout << cl_format("Template `%1` should has only one child-tag!\n", tplName).c_str();

        // Check whether template child's name is equal for parent which called template
        CL_DomElement tplChildElem = templatePointer.get_first_child_element();
        if (parentNodeName != tplChildElem.get_node_name().c_str())
            std::cout << cl_format("You can't apply tpl-child tag `%1` for tag `%2`. Reason: different names!\n", 
                    tplChildElem.get_node_name(), parentNodeName).c_str();

        // Pointer for parent element is in `parent`
        // Pointer for template element is in `templatePointer`
        parent.remove_attribute("template");

        // Work with each child recursively
        processTags(parent, templatePointer.get_first_child(), document);

        // Debug output
        CL_File writeFile("out.xml", CL_File::create_always, CL_File::access_write);
        document.save(writeFile);
    }
}

void SceneLoader::_threadWrapper(const std::string &sceneName)
{
    //mMutex.lock();

    CL_File fileHandle = CL_File("media/levels/" + sceneName);
    CL_DomDocument document(fileHandle);

    CL_DomElement root = document.get_document_element();
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

    if (!world.has_attribute("image")) LOG_NOFORMAT("Error: World tag should have `image` attribute!");
    levelManager().init(world.get_attribute("image"));

    try
    {
        CL_DomNode foregroundNode = environ.select_node("Foreground");

        uint16_t actualSize = 100;
        if (foregroundNode.to_element().has_attribute("actual_size"))
        {
            actualSize = boost::lexical_cast<uint16_t>(foregroundNode.to_element().get_attribute("actual_size").c_str());
        }
        levelManager().setForegroundSize(actualSize);
    }
    catch (...) { }

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
        deps["Foreground"] = Environ_Foreground;
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
            if (tag.get_node_name() == "Foreground")
            {
                levelManager().setForegroundTexture(tag.to_element().get_attribute("image").c_str());
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

            int z_index = 0;
            if (tag.has_attribute("z-index"))
                z_index = boost::lexical_cast<int>(tag.get_attribute("z-index").c_str());

            LOG_NOFORMAT(cl_format("- Parsing object `%1` of type `%2`\n", name, type));

            // TODO: Call template parsing here...

            std::string desc = "";
            Object* object = typesManager().parseObject(&tag, type, desc);
            object->setIndex(z_index);
            if(object != NULL)
            {
                objectsManager().addObject(name, object);
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
