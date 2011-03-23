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
#include "Core/InputManager.hpp"

BOOST_PYTHON_MODULE(InputManager)
{
    bp::class_<InputManager, boost::noncopyable>("InputManager", bp::no_init)
        .def("GetInput", &InputManager::getInput, PYPOLICY_REFERENCE_EXISTING)
        .def("GetKeyboard", &InputManager::getKeyboard, PYPOLICY_REFERENCE_EXISTING)
        .def("GetMouse", &InputManager::getMouse, PYPOLICY_REFERENCE_EXISTING)
        .def("GetJoystick", &InputManager::getJoystick, PYPOLICY_REFERENCE_EXISTING);

    bp::def("getInstance", &inputManager, PYPOLICY_REFERENCE_EXISTING);
}
