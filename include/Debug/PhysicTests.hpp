#ifndef _PHYSIC_TEST_HPP_
#define _PHYSIC_TEST_HPP_

#include "Physic/Body.hpp"
#include "Debug/DebugWatcher.hpp"
#include "Core/LogManager.hpp"

#define physicTests (PhysicTests::get_mutable_instance())

class PhysicTests: public boost::serialization::singleton<PhysicTests>
{
    public:
    void step();
    
    void RunTest();
    
    void SimpleMoisten(BodyPart* part);
    void SimpleHeat(BodyPart* part);
    void SimpleCool(BodyPart* part);
    void SimpleWind(BodyPart* part);
    void SimpleElectricity(BodyPart* part);
    
};

#endif
