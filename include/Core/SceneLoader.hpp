#ifndef _PHYSIC_SCENE_LOADER_HPP_
#define _PHYSIC_SCENE_LOADER_HPP_

#include <boost/serialization/singleton.hpp>
#include <Box2D/Box2D.h>

#include "Core/PhysicManager.hpp"
#include "Core/EnvironManager.hpp"
#include "World/Environ/Types.hpp"

#include "Physic/Body.hpp"
#include <map>

#define sceneLoader (SceneLoader::get_mutable_instance())
#define sceneLoaderConst (SceneLoader::get_const_instance())

class SceneLoader : public
boost::serialization::singleton<SceneLoader>
{
    private:
        // For the threading
        CL_Mutex mMutex;
        CL_Thread mThread;

        void _threadWrapper(const std::string &sceneName);

    public:
        void loadScene(const std::string &sceneName);
};

#endif
