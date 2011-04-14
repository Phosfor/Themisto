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
#include "Core/ResourceManager.hpp"

CL_Sprite (ResourceManager::*getSprite1)(const std::string &, const std::vector<std::string> &) = &ResourceManager::getSprite;
CL_Sprite (ResourceManager::*getSprite2)(const std::string &, const std::string &) = &ResourceManager::getSprite;

int sectionNumChildren(const std::string &section)
{
    return resourceManager().sectionHandle(section).get_child_nodes().get_length();
}

BOOST_PYTHON_MODULE(ResourceManager)
{
    bp::def("SectionNumChildren", &sectionNumChildren);
    bp::class_<ResourceManager, boost::noncopyable>("ResourceManager", bp::no_init)
        .def("GetImagePath", &ResourceManager::getImagePath)
        .def("GetImage", &ResourceManager::getImage)
        .def("GetSprite", getSprite1)
        .def("GetSprite", getSprite2)
        .def("SectionHandle", &ResourceManager::sectionHandle);

    bp::def("getInstance", &resourceManager, PYPOLICY_REFERENCE_EXISTING);
}
