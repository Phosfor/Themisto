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

#include "World/Objects/TypesManager.hpp"

#include "World/Objects/Object.hpp"

boost::python::object TypesManager::empty_parser(CL_DomElement p)
{
    LOG("Error: object type not registered\n");
    return boost::python::object();
}

Parser TypesManager::getParser(std::string type)
{
    if (ObjectsParser.find(type) != ObjectsParser.end())
        return ObjectsParser[type];
    else
        return TypesManager::empty_parser;
}

TypesManager::TypesManager() { }

void TypesManager::dumpParsers()
{
    std::cout << "List of registered parsers:\n";

    std::map<std::string, Parser>::const_iterator it = ObjectsParser.begin();
    for (; it != ObjectsParser.end(); ++it)
    {
        std::cout << "\t" << it->first << "\n";
    }
}

boost::python::object TypesManager::parseObject(CL_DomElement objectTag, const std::string &type)
{
    return (getParser(type))(objectTag);
}

void TypesManager::registerParser(std::string type, Parser p)
{
    if (ObjectsParser.find(type) == ObjectsParser.end())
        ObjectsParser.insert(std::make_pair(type, p));
}
