/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
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
