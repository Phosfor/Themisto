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

#include <map>
#include <utility>
#include <string>

#include <ClanLib/core.h>
#include <boost/serialization/singleton.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>

#include "Core/LogManager.hpp"

typedef boost::function<boost::python::object (CL_DomElement)> Parser;
class TypesManager : public boost::serialization::singleton<TypesManager>
{
    private:
        std::map<std::string, Parser> ObjectsParser;
        static boost::python::object empty_parser(CL_DomElement p);
        Parser getParser(std::string type);

    public:
        TypesManager();
        boost::python::object parseObject(CL_DomElement objectTag, const std::string &type);
        void registerParser(std::string type, Parser p);
        void dumpParsers();
};

inline TypesManager &typesManager() { return TypesManager::get_mutable_instance(); }
