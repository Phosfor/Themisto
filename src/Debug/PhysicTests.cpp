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

#include "Debug/PhysicTests.hpp"
#include "Physic/BodyPart.hpp"
#include "Physic/Impact.hpp"

void PhysicTests::step()
{
}

Body* getBody(BodyPart* part)
{
    return reinterpret_cast<Body*>(part->getFixture()->GetBody()->GetUserData());
}

void PhysicTests::RunTest()
{
    Body* testBody = NULL;
    BOOST_FOREACH(Body* someBody, physicManager().getBodies())
    {
        if(someBody->getName() == "#1")
        {
            testBody = someBody;
            break;
        }
    }
    if(testBody != NULL)
    {
        BodyPart* part = reinterpret_cast<BodyPart*>(testBody->getb2Body()->GetFixtureList()->GetUserData());
        SimpleMoisten(part);
    }
    else
    {
        LOG("Body #1 not found");
    }
}

void PhysicTests::SimpleMoisten(BodyPart* part)
{
    std::string answer;
    /*debugWatcher.parseCommand(cl_format("show material of %1(%2) every eon",
        getBody(part)->getName(), part->getName()), &answer);
         debugWatcher.parseCommand(cl_format("show param of %1(%2) every eon",
        getBody(part)->getName(), part->getName()), &answer); */
    debugWatcher().parseCommand(cl_format("show state(Dampness) of %1(%2) every step",
        getBody(part)->getName(), part->getName()), &answer);
    LOG(answer);

    boost::shared_ptr<Impact> moisten = boost::shared_ptr<Impact>(new Impact(Moisten));
    moisten->Intensity = 0.5;
    part->applyImpact(moisten);
}


void PhysicTests::SimpleHeat(BodyPart* part)
{
}
void PhysicTests::SimpleCool(BodyPart* part)
{
}
void PhysicTests::SimpleWind(BodyPart* part)
{
}
void PhysicTests::SimpleElectricity(BodyPart* part)
{
}
