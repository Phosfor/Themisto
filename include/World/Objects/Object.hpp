#ifndef _WORLD_OBJECT_HPP_
#define _WORLD_OBJECT_HPP_

#include <ClanLib/core.h>
#include "World/Objects/ObjectTypes.hpp"

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
