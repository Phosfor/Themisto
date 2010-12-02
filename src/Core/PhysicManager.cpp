/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Core/PhysicManager.hpp"

PhysicManager::PhysicManager()
{
    b2Vec2 gravity(0.0f, 10.0f);
    mWorld = new b2World(gravity, true);
    mTimeStep = 1.0f / 60.0f;
    mVelocityIterations = 6;
    mPositionIterations = 4;
}

PhysicManager::~PhysicManager()
{
    disposeScene();
}

void PhysicManager::disposeScene()
{
    delete mWorld;
    mWorld = NULL;
}

b2World& PhysicManager::getWorld()
{
    return *mWorld;
}

std::list<Body*> PhysicManager::getBodies()
{
    std::list<Body*> result;
    for(b2Body* body = mWorld->GetBodyList(); body != NULL; body = body->GetNext())
    {
        if(body->GetUserData() != NULL)
        {
            result.push_back((Body*)body->GetUserData());
        }
    }
    return result;
}

void PhysicManager::step()
{
    mWorld->Step(mTimeStep, mVelocityIterations, mPositionIterations);
    mWorld->ClearForces();
}

