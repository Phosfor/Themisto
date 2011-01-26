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

    // Body temperature, after it burning with full strench flame
    float FlameTemperature;

    // How fast body will carbonize
    // value = 0 - flame would make no effect
    // if value=1 then with KindleLevel=maxKindleLevel at unit time inteval, body would fully burn out
    float CarbonizeRate;

    // 0 - wouldn't conducte electicity, 1 - conduct without resistance
    bool ElectricalConductivity;

    // If temperature fall to FrozingTemperature, body would become frozen
    float FrozingTemperature;

    // How much would fall friction, when dumpness grow up to unit
    float InflWetToFriction;

    // How much would fall KinleTemperature, when body become wet
    float InflWetToKindleTemperature;

    // How much would grow up FrozingTemperature, when body become wet
    float InflWetToFrozingTemperature;


    BodyMaterial();
};

#endif
