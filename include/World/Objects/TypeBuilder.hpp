/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

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
