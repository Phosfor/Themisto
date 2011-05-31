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
#include "Core/GuiManager.hpp"

BOOST_PYTHON_MODULE(GuiManager)
{
    bp::class_<GuiManager, boost::noncopyable>("GuiManager", bp::no_init)
        .def("GetHandle", &GuiManager::getHandle, PYPOLICY_REFERENCE_EXISTING)
        .def("GetWM", &GuiManager::getWM, PYPOLICY_REFERENCE_EXISTING)
        .def("GetWrapper", &GuiManager::getWrapper, PYPOLICY_REFERENCE_EXISTING);
        //.def("GetWrapper", &GuiManager::getWrapper, bp::return_value_policy<bp::manage_new_object>());

    bp::def("getInstance", &guiManager, PYPOLICY_REFERENCE_EXISTING);
}
