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
#include "Core/WorldManager.hpp"

BOOST_PYTHON_MODULE(WorldManager)
{
    bp::class_<WorldManager, boost::noncopyable>("WorldManager", bp::no_init)
        .def("GetAbsTime", &WorldManager::getAbsTime)
        .def("SetWorldTime", &WorldManager::setWorldTime)

        .def("SetWindPower", &WorldManager::setWindPower)
        .def("GetWindPower", &WorldManager::getWindPower)

        .def("GenerateUniqueID", &WorldManager::generateUniqueID)
        .def("IsNight", &WorldManager::isNight);

    bp::def("getInstance", &worldManager, PYPOLICY_REFERENCE_EXISTING);
}
