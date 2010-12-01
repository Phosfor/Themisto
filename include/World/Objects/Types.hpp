#ifndef _OBJECT_TYPES_HPP_
#define _OBJECT_TYPES_HPP_

#include <boost/function.hpp>
#include <map>
#include <ClanLib/core.h>

enum ObjectTypes
{
    NotAnObject = 0,
    PhysicBodyObject,
    GrassObject
};

typedef boost::function<void (CL_DomElement*)> parserPointer;
static std::map<ObjectTypes, parserPointer> ObjectsParser;

static inline parserPointer getParserCallback(ObjectTypes &type)
{
    if (ObjectsParser.find(type) != ObjectsParser.end())
        return ObjectsParser[type];
}

#endif
