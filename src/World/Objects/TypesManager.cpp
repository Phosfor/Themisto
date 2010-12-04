/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Objects/TypesManager.hpp"

Object* TypesManager::empty_parser(CL_DomElement* p)
{
    LOG("Empty parser is called.");
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
    registerParser("PhysicBody", Body::ParseBody);
}

Object* TypesManager::parseObject(CL_DomElement* objectTag, const std::string &type)
{
    return (getParser(type))(objectTag);
}

void TypesManager::registerParser(std::string type, Parser p)
{
    if (ObjectsParser.find(type) == ObjectsParser.end())
        ObjectsParser.insert(std::make_pair(type, p));
}
