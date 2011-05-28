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

void KeyDownWrapper(const CL_InputEvent &e1, const CL_InputState &e2, bp::object user_data)
{
    try
    {
        user_data(e1, e2);
    }
    catch(boost::python::error_already_set &e)
    {
        LOG("KeyDownWrapper failed...");
        PyErr_Print();
    }
}
void ConnectKeyDown(bp::object func)
{
    try
    {
        CL_Slot temp = inputManager().getKeyboard().sig_key_down().connect(&KeyDownWrapper, func);
        inputManager().getKeyDownSlots().push_back(temp);
    }
    catch(boost::python::error_already_set &e)
    {
        LOG("ConnectKeyDown failed...");
        PyErr_Print();
    }
}

BOOST_PYTHON_MODULE(InputManager)
{
    bp::class_<InputManager, boost::noncopyable>("InputManager", bp::no_init)
        .def("GetInput", &InputManager::getInput, PYPOLICY_REFERENCE_EXISTING)
        .def("GetKeyboard", &InputManager::getKeyboard, PYPOLICY_REFERENCE_EXISTING)
        .def("GetMouse", &InputManager::getMouse, PYPOLICY_REFERENCE_EXISTING)
        .def("GetJoystick", &InputManager::getJoystick, PYPOLICY_REFERENCE_EXISTING)

        .def("ConnectKeyDown", &ConnectKeyDown)
        .staticmethod("ConnectKeyDown")

        .def("IsKeyPressed", &InputManager::keyPressed);

    bp::def("getInstance", &inputManager, PYPOLICY_REFERENCE_EXISTING);

    // Keys
    bp::scope().attr("CL_KEY_D") = CL_KEY_D;
    bp::scope().attr("CL_KEY_CONTROL") = CL_KEY_CONTROL;
    bp::scope().attr("CL_KEY_RIGHT") = CL_KEY_RIGHT;
    bp::scope().attr("CL_KEY_LEFT") = CL_KEY_LEFT;
    bp::scope().attr("CL_KEY_UP") = CL_KEY_UP;
    bp::scope().attr("CL_KEY_DOWN") = CL_KEY_DOWN;
}
