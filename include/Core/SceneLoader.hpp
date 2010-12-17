/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _PHYSIC_SCENE_LOADER_HPP_
#define _PHYSIC_SCENE_LOADER_HPP_

#include <boost/serialization/singleton.hpp>
#include <Box2D/Box2D.h>

#include "Core/PhysicManager.hpp"
#include "Core/EnvironManager.hpp"
#include "Core/ObjectsManager.hpp"
#include "World/Environ/Types.hpp"
#include "World/Objects/TypesManager.hpp"
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

        void copyAttributes(CL_DomNode parent, CL_DomNode templatePointer);
        void processTags(CL_DomNode parent, CL_DomNode templatePointer, CL_DomDocument document);
        void processTemplates(CL_DomDocument document);

    public:
        void loadScene(const std::string &sceneName);
};

#endif
