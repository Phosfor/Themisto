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

#include "Physic/Impact.hpp"

Impact::Impact(ImpactTypes type)
{
    Type = type;
}

Impact::Impact(ImpactTypes type, float intensity, b2Vec2 impactPoint, b2Vec2 dirrection)
{
    Type = type;
    Intensity = intensity;
    impactPoint  = impactPoint;
    Dirrection = dirrection;
}
