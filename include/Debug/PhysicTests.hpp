/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _PHYSIC_TEST_HPP_
#define _PHYSIC_TEST_HPP_

#include "Physic/Body.hpp"
#include "Debug/DebugWatcher.hpp"
#include "Core/LogManager.hpp"

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

inline PhysicTests &physicTests() { return PhysicTests::get_mutable_instance(); }

#endif /* _PHYSIC_TESTS_HPP_ */
