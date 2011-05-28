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

#include "Core/ScriptsManager.hpp"
#include "Core/LevelManager.hpp"
#include "Core/SceneLoader.hpp"

void LoadScene(const std::string &name)
{
    sceneLoader().loadScene(name);
}

BOOST_PYTHON_MODULE(LevelManager)
{
    bp::class_<LevelManager, boost::noncopyable>("LevelManager", bp::no_init)
        .def("Init", &LevelManager::init)
        .def("initObjects", &LevelManager::initObjects)

        .def("GetCamera", &LevelManager::getCamera, PYPOLICY_REFERENCE_EXISTING)

        .def("AddObject", &LevelManager::addObject)
        .def("GetObject", &LevelManager::getPyObject)
        .def("GetObjectByType", &LevelManager::getPyObjectByType)
        .def("GetObjectsByType", &LevelManager::getPyObjectsByType)

        .def("UpdateVisual", &LevelManager::updateVisual)
        .def("UpdateLogic", &LevelManager::updateLogic);

    bp::def("getInstance", &levelManager, PYPOLICY_REFERENCE_EXISTING);
    bp::def("LoadScene", &LoadScene);
}
