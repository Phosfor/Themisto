/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

class Object : public boost::noncopyable
{
    protected:
        std::string mName;
        ObjectTypes mType;
        int z_index;

    public:
        virtual std::string getName() { return mName; }
        virtual void setName(const std::string &name) { mName = name; }

        virtual int getIndex() { return z_index; }
        virtual void setIndex(int _z_index) { z_index = _z_index; }

        virtual ObjectTypes getType() { return mType; }

        virtual CL_Pointf getPosition() = 0;
        virtual CL_Rectf getRectangle() = 0;

        virtual void update(float elapsed) = 0;
        virtual void updateVisual(float newX, float newY) = 0;
};

#endif /* _WORLD_OBJECT_HPP_ */
