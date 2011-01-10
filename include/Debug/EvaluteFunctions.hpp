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

#ifndef _EVALUTE_FUNCTIONS_HPP_
#define _EVALUTE_FUNCTIONS_HPP_

#include "Core/ApplicationManager.hpp"
#include "Core/Utils.hpp"

class EnvironObject;
class BodyMaterial;
class BodyState;
class BodyPart;
class Watch;
namespace evalute
{
    const int BodyMaterialFieldsCount = 20;
    const std::string BodyMaterialFields[] = {
        "Name",
        "KindleTemperature",
        "KindleReceptivity",
        "FlameTemperature",
        "SelfFlareUpRate",
        "CarbonizeRate",
        "ElectricalConductivity",
        "ThermalReceptivity",
        "DampReceptivity",
        "FrozingTemperature",
        "InflDampnessToFriction",
        "InflDampnessToKindleTemperature",
        "InflDampnessToMaxKindle",
        "InflDampnessToKindleReceptivity",
        "InflDampnessToFrozingTemperature",
        "InflCarbonizeLevelToMaxKindle",
        "InflCarbonizeLevelToMaxDampness",
        "InflCarbonizeLevelToElecticalConductivity",
        "InflMoistenToKindleLevel",
        "InflTemperatureToDampness"
    };

    const int BodyStateFieldsCount = 5;
    const std::string BodyStateFields[] = {
        "IsFrozen",
        "KindleLevel",
        "CarbonizeLevel",
        "Temperature",
        "Dampness"
    };

    const int b2FixtureFieldsCount = 23;
    const std::string b2FixtureFields[] = {
        "Shape",
        "Shape.Type",
        "Shape.VertexCount",
        "Shape.Vertices",
        "Shape.Radius",
        "Shape.Center",
        "IsSensor",
        "Filter",
        "Filter.CategoryBits",
        "Filter.MaskBits",
        "Filter.GroupIndex",
        "ParentBody",
        "Mass",
        "Mass.Center",
        "Mass.RotationInertia",
        "Density",
        "Restitution",
        "Friction",
        "AABB",
        "AABB.Top",
        "AABB.Bottom",
        "AABB.Left",
        "AABB.Right"
    };

    const int b2BodyFieldsCount = 18;
    const std::string b2BodyFields[] = {
        "Type",
        "Position",
        "Angle",
        "WorldCenter",
        "LocalCenter",
        "LinearVelocity",
        "AngularVelocity",
        "Mass",
        "Mass.Center",
        "Mass.RotationInertia",
        "Inertia",
        "LinearDamping",
        "AngularDamping",
        "Bullet",
        "SleepingAllowed",
        "Awake",
        "Active",
        "FixedRotation"
    };

    const int BodyPartFieldsCount = 5;
    const std::string BodyPartFields[] = {
        "IsDefaultMaterial",
        "MaxKindleLevel",
        "MaxDampness",
        "AcceptsCord",
        "Name"
    };

    const int BodyFieldsCount = 2;
    const std::string BodyFields[] = {
        "Buoyancy",
        "Name",
    };

    const int EnvironObjectFieldsCount = 2;
    const std::string EnvironObjectFields[] = {
        "Enabled",
        "Limit"
    };

    std::string evalute_material(Watch* watch);
    std::string evalute_state(Watch* watch);
    std::string evalute_b2Fixture(Watch* watch);
    std::string evalute_b2Body(Watch* watch);
    std::string evalute_BodyPart(Watch* watch);
    std::string evalute_Body(Watch* watch);
    std::string evalute_EnvironObject(Watch* watch);
    std::string evalute_elapsed(Watch* watch);
}

#endif /* _EVALUTE_FUNCTIONS_HPP */
