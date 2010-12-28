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

#ifndef _IMPACT_HPP_
#define _IMPACT_HPP_

#include <ClanLib/core.h>
#include <Box2D/Common/b2Math.h>

enum ImpactTypes
{
    Nothing = 0,
    Moisten = 1,
    Heat = 2,
    Cool = 3,
    Wind = 5,
    Electricity = 6
};

struct Impact
{
    ImpactTypes Type;
    // Moisten: ammount of water per time unit
    // Heat, Cool: delta temperature per time unit, for body with ThermalCapacity=1
    // Beat: impuls
    // Wind: ?
    // Electicity: ?
    float Intensity;
    b2Vec2 ImpactPoint;
    b2Vec2 Dirrection;

    Impact(ImpactTypes type);
    Impact(ImpactTypes type, float intensity, b2Vec2 impactPoint, b2Vec2 dirrection);
};

#endif /* _IMPACT_HPP_ */
