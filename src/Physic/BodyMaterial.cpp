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

#include "Physic/BodyMaterial.hpp"


BodyMaterial::BodyMaterial()
{
    Name = "Default";
    KindleTemperature = 10000;
    FlameTemperature = 0;
    CarbonizeRate = 0;
    ElectricalConductivity = true;
    FrozingTemperature = -273;
    InflDampnessToFriction = 0;
    InflDampnessToKindleTemperature = 0;

    InflDampnessToThermalReceptivity = 0;
    InflDampnessToFrozingTemperature = 0;
}
