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

#pragma once

#include <fstream>
#include <iostream>

#include <ClanLib/core.h>
#include <boost/python.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/filesystem.hpp>

#include "Core/LogManager.hpp"
#include "Core/TypesConverters.hpp"

namespace bp = boost::python;

class ScriptsManager : public boost::serialization::singleton<ScriptsManager>
{
    private:
        bp::object mMainModule, mMainNamespace;

        void processPaths();
        void registerTypesConverters();

    public:
        ScriptsManager();
        ~ScriptsManager();

        bp::object &getMainModule();
        bp::object &getMainNamespace();

        void runString(const std::string &pyCode);
        void runFile(const std::string &fileName);
};


inline ScriptsManager &scriptsManager() { return ScriptsManager::get_mutable_instance(); }
