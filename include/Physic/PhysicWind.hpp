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

#ifndef _PHYSIC_WIND_HPP_
#define _PHYSIC_WIND_HPP_

#include <Box2D/Box2D.h>
#include <ClanLib/core.h>

class Impact;
class BodyPart;
class PhysicWind
{
    private:


    public:
        void setPower(float value);
        void setDirrection(b2Vec2 value);
        void init(float power, b2Vec2 dirrection);
};

#endif /* _PHYSIC_WIND_HPP_ */
