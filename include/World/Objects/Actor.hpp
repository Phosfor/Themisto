/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

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

#include "World/Objects/Object.hpp"

class Body;

class Actor : public Object
{
    protected:
        Actor() {}
    public:
        virtual void goLeft() = 0;
        virtual void goRight() = 0;
        virtual void stopMoving() = 0;
        virtual void beginJump() = 0;
        virtual void endJump() = 0;
        virtual boost::shared_ptr<Body> getShoulder() = 0;
        virtual b2RevoluteJoint* getShoulderJoint() = 0;
        virtual boost::shared_ptr<Body> getTrunk() = 0;
        virtual b2RevoluteJoint* getTrunkJoint() = 0;
};
