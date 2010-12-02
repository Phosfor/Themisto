/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _EVALUTE_FUNCTIONS_HPP_
#define _EVALUTE_FUNCTIONS_HPP_

#include "Debug/Watch.hpp"
#include "World/Objects/Body.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"
#include "Core/Utils.hpp"

namespace evalute
{
    const int BodyMaterialFieldsCount = 20;
    const string BodyMaterialFields[] = {
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
    const string BodyStateFields[] = {
        "IsFrozen",
        "KindleLevel",
        "CarbonizeLevel",
        "Temperature",
        "Dampness"        
    };
    
    const int b2FixtureFieldsCount = 23;
    const string b2FixtureFields[] = {
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
    const string b2BodyFields[] = {
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
    const string BodyPartFields[] = {
        "IsDefaultMaterial",
        "MaxKindleLevel",
        "MaxDampness",
        "AcceptsCord",
        "Name"
    };
    
    const int BodyFieldsCount = 2;
    const string BodyFields[] = {
        "Buoyancy",
        "Name",
    };
    
    const int EnvironObjectFieldsCount = 2;
    const string EnvironObjectFields[] = {
        "Enabled",
        "Limit"
    };

    string evalute_material(Watch* watch);
    string evalute_state(Watch* watch);
    string evalute_b2Fixture(Watch* watch);
    string evalute_b2Body(Watch* watch);
    string evalute_BodyPart(Watch* watch);
    string evalute_Body(Watch* watch);
    string evalute_EnvironObject(Watch* watch);
    string evalute_elapsed(Watch* watch);

}

#endif
