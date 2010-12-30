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

#ifndef _PHYSIC_TEST_HPP_
#define _PHYSIC_TEST_HPP_

#include "Physic/Body.hpp"
#include "Debug/DebugWatcher.hpp"
#include "Core/LogManager.hpp"

class BodyPart;
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
