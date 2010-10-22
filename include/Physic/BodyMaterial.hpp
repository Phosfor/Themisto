#ifndef _BODY_MATERIAL_HPP_
#define _BODY_MATERIAL_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/singleton.hpp>

#include <vector>

using boost::property_tree::ptree;
using namespace std;

struct BodyMaterial
{
    string Name;

    //if temperature become more then kindle temperature, body would flare up
    float KindleTemperature;

    //how fast body will flare up
    //value=0:, mKindleLevel always would be 0
    //value=1: under heat impact with intensity=KindleTemperature, at unit time interval,
    //KindleLevel would become MaxKindleLevel
    float KindleReceptivity;

    //body temperature, after it burning with full strench flame
    float FlameTemperature;

    //How fast body will flare up by itself
    //value 0: body wouldn't flare up by itself
    //value 1: if KindleLevel > 0 then at unit time interval KindleLevel become MaxKindleLevel
    float SelfFlareUpRate;

    //how fast body will carbonize
    //value = 0 - flame would make no effect
    //if value=1 then with KindleLevel=maxKindleLevel at unit time inteval, body would fully burn out
    float CarbonizeRate;

    //0 - wouldn't conducte electicity, 1 - conduct without resistance
    float ElectricalConductivity;

    //How fast body would warm up
    //0 - wouln't warm up, 1 - receive all warm
    float ThermalReceptivity;

    //How fost body would soak water
    //if value 0, then Dumpness would always be 0
    //if value 1, then under moisten impact with intensity=1,
    //Dampness would become MaxDumpness in one time unit
    float DampReceptivity;

    //if temperature fall to FrozingTemperature, body would become frozen
    float FrozingTemperature;

    BodyMaterial();
};


#endif
