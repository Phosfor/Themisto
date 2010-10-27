#include "Core/SceneLoader.hpp"

void SceneLoader::loadScene(const std::string &sceneName)
{
    mThread.start<SceneLoader, const std::string&>(this, &SceneLoader::_threadWrapper, sceneName);
    mThread.join();
}

void SceneLoader::_threadWrapper(const std::string &sceneName)
{
    mMutex.lock();

    CL_File fileHandle = CL_File("media/levels/" + sceneName);
    CL_DomDocument document(fileHandle);
    CL_DomElement root = document.get_document_element();

    CL_String authors = root.get_attribute("authors");
    CL_String version = root.get_attribute("version");

    LOG(cl_format("Loading new level with name `%1` for the game version: `%2`. Created by: `%3`."
                  , sceneName, version, authors));

    if (boost::lexical_cast<float>(version.c_str()) != GAME_VERSION)
        LOG(cl_format("Warning! Level version is different with game one(%1). Possible faults.", version));

    mMutex.unlock();
}
