#ifndef _WORLD_OBJECT_HPP_
#define _WORLD_OBJECT_HPP_

#include <ClanLib/core.h>

class Object
{
    protected:
        float mX, mY;
        std::string mName, mType;

    public:
        virtual std::string getName() { return mName; }
        virtual void setName(const std::string &name) { mName = name; }

        virtual std::string getType() { return mType; }
        virtual void setType(const std::string &type) { mType = type; }

        virtual void setPosition(CL_Pointf &pos) { mX = pos.x; mY = pos.y; }
        virtual void setX(float x) { mX = x; }
        virtual void setY(float y) { mY = y; }

        virtual CL_Pointf getPosition() { return CL_Pointf(mX, mY); }
        virtual float getX() { return mX; }
        virtual float getY() { return mY; }

        virtual void update() = 0;
};

#endif /* _WORLD_OBJECT_HPP_ */
