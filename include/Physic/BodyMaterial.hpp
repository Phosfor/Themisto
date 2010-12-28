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

#ifndef _BODY_MATERIAL_HPP_
#define _BODY_MATERIAL_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/singleton.hpp>

#include <vector>

struct BodyMaterial
{
    std::string Name;

    // If temperature become more then kindle temperature, body would flare up
    float KindleTemperature;

    // How fast body will flare up
    // value=0:, mKindleLevel always would be 0
    // value=1: under heat impact with intensity=KindleTemperature, at unit time interval,
    // KindleLevel would become MaxKindleLevel
    float KindleReceptivity;

    // Body temperature, after it burning with full strench flame
    float FlameTemperature;

    // How fast body will flare up by itself
    // value 0: body wouldn't flare up by itself
    // value 1: if KindleLevel > 0 then at unit time interval KindleLevel become MaxKindleLevel
    float SelfFlareUpRate;

    // How fast body will carbonize
    // value = 0 - flame would make no effect
    // if value=1 then with KindleLevel=maxKindleLevel at unit time inteval, body would fully burn out
    float CarbonizeRate;

    // 0 - wouldn't conducte electicity, 1 - conduct without resistance
    float ElectricalConductivity;

    // How fast body would warm up
    // 0 - wouln't warm up, 1 - receive all warm
    float ThermalReceptivity;

    // How fost body would soak water
    // if value 0, then Dumpness would always be 0
    // if value 1, then under moisten impact with intensity=1,
    // Dampness would become MaxDumpness in one time unit
    float DampReceptivity;

    // If temperature fall to FrozingTemperature, body would become frozen
    float FrozingTemperature;

    // How much would fall friction, when dumpness grow up to unit
    float InflDampnessToFriction;

    // How much would fall KinleTemperature, when dumpness grow up to unit
    float InflDampnessToKindleTemperature;

    // How much would fall MaxKindle, when dumpness grow up to unit
    float InflDampnessToMaxKindle;

    // How much would fall KinleReceptivity, when dumpness grow up to unit
    float InflDampnessToKindleReceptivity;

    // How much would grow up FrozingTemperature, when dumpness grow up to unit
    float InflDampnessToFrozingTemperature;

    // How much would fall MaxKindle, when CarbonizeLevel grow up to one per cent
    float InflCarbonizeLevelToMaxKindle;

    // How much would fall MaxDumpness, when CarbonizeLevel grow up to one per cent
    float InflCarbonizeLevelToMaxDampness;

    // How much would fall ElecticalConductivity, when CarbonizeLevel grow up to one per cent
    float InflCarbonizeLevelToElecticalConductivity;

    // How mush would fall KindleLevel becouse of an one unit of water
    float InflMoistenToKindleLevel;

    // How many water would evaporate in time unit, when temperature = 1
    float InflTemperatureToDampness;

    BodyMaterial();
};

#endif
