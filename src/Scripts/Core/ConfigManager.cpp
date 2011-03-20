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
#include "Core/ConfigManager.hpp"

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getValueIntOverloads, ConfigManager::getValue<int>, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getValueStringOverloads, ConfigManager::getValue<std::string>, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getValueFloatOverloads, ConfigManager::getValue<float>, 1, 2);

BOOST_PYTHON_MODULE(ConfigManager)
{
    bp::class_<ConfigManager, boost::noncopyable>("ConfigManager", bp::no_init)
        .def("SetPath", &ConfigManager::setPath)
        .def("GetListValue", &ConfigManager::getListValue)
        .def("GetValue_i", &ConfigManager::getValue<int>, getValueIntOverloads())
        .def("GetValue_s", &ConfigManager::getValue<std::string>, getValueStringOverloads())
        .def("GetValue_f", &ConfigManager::getValue<float>, getValueFloatOverloads());

    bp::def("getInstance", &configManager, PYPOLICY_REFERENCE_EXISTING);
}
