#ifndef _WORLD_OBJECT_TYPE_BUILDER_HPP_
#define _WORLD_OBJECT_TYPE_BUILDER_HPP_

#include "World/Objects/Object.hpp"

// Objects
#include "World/Objects/Grass.hpp"

inline Object *BuildObjectType(const std::string &typeName)
{
    if (typeName == "Object_Grass")
        return new Grass;
    //else if (typeName == )
}

#endif /* _WORLD_OBJECT_TYPE_BUILDER_HPP_ */
