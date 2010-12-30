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

class Object;
class SceneLoader : public boost::serialization::singleton<SceneLoader>
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

inline SceneLoader &sceneLoader() { return SceneLoader::get_mutable_instance(); }

#endif /* _PHYSIC_SCENE_LOADER_HPP_ */
