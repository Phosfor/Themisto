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

#include "Physic/BodyMaterial.hpp"


BodyMaterial::BodyMaterial()
{
    Name = "Default";
    KindleTemperature = 10000;
    KindleReceptivity = 0;
    FlameTemperature = 0;
    SelfFlareUpRate = 0;
    CarbonizeRate = 0;
    ElectricalConductivity = 0;
    ThermalReceptivity = 1;
    DampReceptivity = 0;
    FrozingTemperature = -273;
    InflDampnessToFriction = 0;
    InflDampnessToKindleTemperature = 0;
    InflDampnessToMaxKindle = 0;
    InflDampnessToKindleReceptivity = 0;
    InflDampnessToFrozingTemperature = 0;
    InflCarbonizeLevelToMaxKindle = 0;
    InflCarbonizeLevelToMaxDampness = 0;
    InflCarbonizeLevelToElecticalConductivity = 0;
    InflMoistenToKindleLevel = 0;
    InflTemperatureToDampness = 0;
}
