#include "Debug/PhysicTests.hpp"

void PhysicTests::step()
{
}

Body* getBody(BodyPart* part)
{
    return (Body*)part->getFixture()->GetBody()->GetUserData();
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
        BodyPart* part = (BodyPart*)testBody->getb2Body()->GetFixtureList()->GetUserData();
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

    Impact* moisten = new Impact(Moisten);
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
