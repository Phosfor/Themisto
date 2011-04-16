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

BOOST_PYTHON_MODULE(LevelManager)
{
    bp::class_<LevelManager, boost::noncopyable>("LevelManager", bp::no_init)
        .def("SetCamViewport", &LevelManager::setCamViewport)
        .def("GetCamViewport", &LevelManager::getCamViewport)

        .def("TranslateCamera", &LevelManager::translateCamera)
        .def("GetAbsoluteCameraPos", &LevelManager::getAbsoluteCameraPos)

        .def("GetCameraSpeed", &LevelManager::getCamViewport)
        .def("SetCameraSpeed", &LevelManager::setCamViewport)

        .def("GetDrawDebugData", &LevelManager::getDrawDebugData)
        .def("SetDrawDebugData", &LevelManager::setDrawDebugData)

        .def("Init", &LevelManager::init)
        .def("initObjects", &LevelManager::initObjects)

        .def("AddObject", &LevelManager::addObject)
        .def("GetObject", &LevelManager::getPyObject)
        .def("GetObjectByType", &LevelManager::getPyObjectByType)
        .def("GetObjectsByType", &LevelManager::getPyObjectsByType)

        .def("UpdateVisual", &LevelManager::updateVisual)
        .def("UpdateLogic", &LevelManager::updateLogic);

    bp::def("getInstance", &levelManager, PYPOLICY_REFERENCE_EXISTING);
}
