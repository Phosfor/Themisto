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
#include "Core/Utils.hpp"

BOOST_PYTHON_MODULE(ConfigManager)
{
    bp::def("getValue", ScriptsConfig::getConfigValue);
    bp::def("setPath", ScriptsConfig::setConfigPath);

    bp::to_python_converter<std::vector<std::string,class std::allocator<std::string> >, VecToList<std::string> >();
    bp::def("getListValue", ScriptsConfig::getConfigListValue);
}
