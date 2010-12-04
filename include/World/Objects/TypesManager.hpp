/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _OBJECT_TYPES_HPP_
#define _OBJECT_TYPES_HPP_

#include "Core/LogManager.hpp"
#include "World/Objects/Object.hpp"
#include "World/Objects/Body.hpp"
#include <boost/function.hpp>
#include <map>
#include <utility>
#include <ClanLib/core.h>
#include <string>
#include <boost/serialization/singleton.hpp>

typedef boost::function<Object* (CL_DomElement*)> Parser;

#define typesManager (TypesManager::get_mutable_instance())
#define typesManagerConst (TypesManager::get_const_instance())

class TypesManager : public boost::serialization::singleton<TypesManager>
{
    private:
        std::map<std::string, Parser> ObjectsParser;
        static Object* empty_parser(CL_DomElement* p);
        Parser getParser(std::string type);

    public:
        TypesManager();
        Object* parseObject(CL_DomElement* objectTag, const std::string &type);
        void registerParser(std::string type, Parser p);
};
#endif
