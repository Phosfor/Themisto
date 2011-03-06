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
#include "World/Objects/PhysicObject.hpp"
#include "World/Objects/Level.hpp"
#include "World/Objects/LightColumn.hpp"
#include "World/Objects/Moon.hpp"
#include "World/Objects/Player.hpp"
#include "World/Objects/Birds.hpp"
#include "World/Objects/Clouds.hpp"
#include "World/Objects/Rain.hpp"
#include "World/Objects/Foreground.hpp"
#include "World/Objects/Empty.hpp"

boost::shared_ptr<Object> TypesManager::empty_parser(CL_DomElement* p, std::string& desc)
{
    desc += "Error: object type not registered.";
    LOG("Error: object type not registered\n");
    return boost::shared_ptr<Object>();
}

Parser TypesManager::getParser(std::string type)
{
    // Check whether some object should have common type but it should be marked
    if (type.find_first_of("_") != std::string::npos)
        type = type.substr(0, type.find_first_of("_"));

    if (ObjectsParser.find(type) != ObjectsParser.end())
        return ObjectsParser[type];
    else
        return TypesManager::empty_parser;
}

TypesManager::TypesManager()
{
    registerParser("PhysicBodyObject", PhysicObject::ParsePhysicObject);
    registerParser("LightColumnObject", LightColumn::ParseLightColumn);
    registerParser("Moon", Moon::ParseMoon);
    registerParser("Player", Player::ParsePlayer);
    registerParser("Birds", Birds::ParseBirds);
    registerParser("Clouds", Clouds::ParseClouds);
    registerParser("Rain", Rain::ParseRain);
    registerParser("Foreground", Foreground::ParseForeground);
    registerParser("Level", Level::ParseLevel);
}


boost::shared_ptr<Object> TypesManager::parseObject(CL_DomElement* objectTag, const std::string &type, std::string &desc)
{
    return (getParser(type))(objectTag, desc);
}

void TypesManager::registerParser(std::string type, Parser p)
{
    if (ObjectsParser.find(type) == ObjectsParser.end())
        ObjectsParser.insert(std::make_pair(type, p));
}
