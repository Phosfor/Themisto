#include "Core/SceneLoader.hpp"

void SceneLoader::loadScene(const std::string &sceneName)
{
    //mThread.start<SceneLoader, const std::string&>(this, &SceneLoader::_threadWrapper, sceneName);
    //mThread.join();
    _threadWrapper(sceneName);
}

void SceneLoader::_threadWrapper(const std::string &sceneName)
{
    //mMutex.lock();

    CL_File fileHandle = CL_File("media/levels/" + sceneName);
    CL_DomDocument document(fileHandle);

    CL_DomElement root = document.get_document_element();
    CL_DomElement world = root.get_first_child_element();
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
    LOG_NOFORMAT(cl_format("- Environ enabled state: %1\n", mEnvironEnabled));

    // If Environ is enabled, go through all environ-params in the level file
    if (mEnvironEnabled)
    {
        using namespace boost;
        environManager.initEnviron();

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
                float pow = lexical_cast<float>(tag.to_element().get_attribute("power").c_str());
                environManager.setWindPower(pow);
            }
            EnvironTypes type = deps[tag.get_node_name().c_str()];

            bool enabled = tag.to_element().get_attribute("enabled") == "true";

            int lim = -1;
            CL_String s_lim = tag.to_element().get_attribute("limit");
            if (s_lim != "") lim = lexical_cast<int>(s_lim.c_str());

            environManager.enableType(enabled, type, lim);
        }
        LOG_NOFORMAT("- All environ objects are loaded.\n");
    }

    // Objects parsing
    {
        using namespace boost;

        float x = 0, y = 0;

        CL_DomNodeList childList = objects.get_child_nodes();
        for (int i=0; i < childList.get_length(); ++i)
        {
            CL_DomElement tag = childList.item(i).to_element();

            std::string name = tag.get_attribute("name").c_str();
            std::string type = tag.get_attribute("type").c_str();
            bool physic = tag.get_attribute("physic_body") == "true";
            LOG_NOFORMAT(cl_format("- Parsing object `%1` of type `%2`, physic body: `%3`\n", 
                        name, type, physic));

            // Parsing visuals
            {
                CL_DomElement visual = objects.get_elements_by_tag_name("Visual").item(0).to_element();
                CL_DomNodeList childList = visual.get_child_nodes();

                for (int i=0; i < childList.get_length(); ++i)
                {
                    CL_DomElement tag = childList.item(i).to_element();
                    if (tag.get_node_name() == "Position")
                    {
                        x = lexical_cast<float>(tag.get_attribute("x").c_str());
                        y = lexical_cast<float>(tag.get_attribute("y").c_str());
                    }
                }
            }

            // Creating final object
            Object *obj = BuildObjectType(type);
            obj->setPosition(CL_Pointf(x, y));
            obj->setName(name);
            objectsManager.addObject(name, obj);
        }
    }

    //mMutex.unlock();
}
