#include "Core/PhysicManager.hpp"

PhysicManager::PhysicManager()
{
    b2Vec2 gravity(0.0f, 10.0f);
    mWorld = new b2World(gravity, false);
    mTimeStep = 1.0f / 60.0f;
    mVelocityIterations = 6;
    mPositionIterations = 4;
    mBodies = new std::list<Body*>;
}

PhysicManager::~PhysicManager()
{
    disposeScene();

    delete mBodies;
    delete mWorld;
}

void PhysicManager::disposeScene()
{
    Body* b ;
    for (std::list<Body*>::iterator body=mBodies->begin(); body!=mBodies->end(); )
    {
        b = *body;
        ++body;
        mBodies->remove(b);

        delete b;

    };
    mBodies->clear();
}

b2World& PhysicManager::getWorld()
{
    return *mWorld;
}

void PhysicManager::registerBody(Body* body)
{
    mBodies->push_front(body);
}

std::list<Body*>& PhysicManager::getBodies()
{
    return *mBodies;
}

void PhysicManager::step()
{
    for (std::list<Body*>::iterator body=mBodies->begin(); body!=mBodies->end(); ++body)
    {
        (*body)->step(mTimeStep);
    }
    mWorld->Step(mTimeStep, mVelocityIterations, mPositionIterations);
    mWorld->ClearForces();
}

void PhysicManager::updateVisual()
{
    for (std::list<Body*>::iterator body=mBodies->begin(); body!=mBodies->end(); ++body)
    {
        (*body)->updateVisual();
    }
}
