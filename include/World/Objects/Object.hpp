/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _WORLD_OBJECT_HPP_
#define _WORLD_OBJECT_HPP_

#include <ClanLib/core.h>

enum ObjectTypes
{
    NotAnObject = 0,
    PhysicBodyObject,
    GrassObject
};

class Object
{
    protected:
        std::string mName;
        ObjectTypes mType;

    public:
    
        virtual std::string getName() { return mName; }
        virtual void setName(const std::string &name) { mName = name; }

        virtual ObjectTypes getType() { return mType; }

        virtual CL_Pointf getPosition() = 0;

        virtual void update(float elapsed) = 0;
};

#endif /* _WORLD_OBJECT_HPP_ */
