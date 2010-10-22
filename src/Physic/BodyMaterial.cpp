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
}
