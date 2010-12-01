#ifndef _OBJECT_TYPES_HPP_
#define _OBJECT_TYPES_HPP_

#include <boost/function.hpp>
#include <map>
#include <utility>
#include <ClanLib/core.h>

enum ObjectTypes
{
    NotAnObject = 0,
    PhysicBodyObject,
    GrassObject
};

typedef boost::function<void (CL_DomElement*)> parserPointer;
static std::map<string, parserPointer> ObjectsParser;

//void empty_parser(CL_DomElement* p)
//{
//}

static inline parserPointer getParserCallback(string type)
{
    if (ObjectsParser.find(type) != ObjectsParser.end())
    {
        return ObjectsParser[type];
    }
    //else
    //{
        //return empty_parser;
    //}
}

static inline void insertParserCallback(string type, parserPointer &p)
{
    if (ObjectsParser.find(type) != ObjectsParser.end())
        ObjectsParser.insert(std::make_pair(type, p));
}

#endif
