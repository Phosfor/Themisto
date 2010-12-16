/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Objects/TypesManager.hpp"

Object* TypesManager::empty_parser(CL_DomElement* p, std::string& desc)
{
    desc += "Error: object type not registered.";
    return NULL;
}

Parser TypesManager::getParser(std::string type)
{
    if (ObjectsParser.find(type) != ObjectsParser.end())
        return ObjectsParser[type];
    else
        return TypesManager::empty_parser;
}

TypesManager::TypesManager()
{
    registerParser("PhysicBodyObject", PhysicObject::ParsePhysicObject);
}

Object* TypesManager::parseObject(CL_DomElement* objectTag, const std::string &type, std::string &desc)
{
    return (getParser(type))(objectTag, desc);
}

void TypesManager::registerParser(std::string type, Parser p)
{
    if (ObjectsParser.find(type) == ObjectsParser.end())
        ObjectsParser.insert(std::make_pair(type, p));
}
