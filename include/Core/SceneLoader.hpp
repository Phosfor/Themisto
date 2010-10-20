#ifndef _PHYSIC_SCENE_LOADER_HPP_
#define _PHYSIC_SCENE_LOADER_HPP_

#include <boost/serialization/singleton.hpp>
#include <Box2D/Box2D.h>

#define sceneLoader (SceneLoader::get_mutable_instance())
#define sceneLoaderConst (SceneLoader::get_const_instance())

using namespace std;

class SceneLoader : public 
boost::serialization::singleton<SceneLoader>
{
    public:
        SceneLoader();
        void loadScene();
};

#endif
