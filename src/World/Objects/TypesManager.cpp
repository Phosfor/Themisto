/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Objects/TypesManager.hpp"

Object* TypesManager::empty_parser(CL_DomElement* p)
{
    LOG("empty parser");
    return NULL;
}


Parser TypesManager::getParser(std::string type)
{
    if (ObjectsParser.find(type) != ObjectsParser.end())
    {
        return ObjectsParser[type];
    }
    else
    {
        Parser p = TypesManager::empty_parser;
        return p;
    }
}


TypesManager::TypesManager()
{
    registerParser("PhysicBody", Body::ParseBody);
}
   
Object* TypesManager::parseObject(CL_DomElement* objectTag)
{
    std::string type = objectTag->get_attribute("type").c_str();
    Parser parser = getParser(type);
    return parser(objectTag);
}
void TypesManager::registerParser(std::string type, Parser p)
{
    if (ObjectsParser.find(type) == ObjectsParser.end())
        ObjectsParser.insert(std::make_pair(type, p));
}
