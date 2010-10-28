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
    CL_DomElement environ = world.get_first_child_element();

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

            environManager.enableType(enabled, type);
        }
        LOG_NOFORMAT("- All environ objects are loaded.\n");
    }

    //mMutex.unlock();
}
