/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

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

#include "Core/PhysicManager.hpp"


PhysicManager::PhysicManager()
{
    b2Vec2 gravity(0.0f, 10.0f);
    mWorld = new b2World(gravity, true);
    mTimeStep = 1.0f / 60.0f;
    mVelocityIterations = 7;
    mPositionIterations = 5;
    mAccomulated = 0;
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

std::list<PhysicObject*> PhysicManager::getBodies()
{
    std::list<PhysicObject*> result;
    for(b2Body* body = mWorld->GetBodyList(); body != NULL; body = body->GetNext())
    {
        if(body->GetUserData() != NULL)
        {
            result.push_back(reinterpret_cast<PhysicObject*>(body->GetUserData()));
        }
    }
    return result;
}
float mAccomulated = 0;
void PhysicManager::step()
{
    float elapsed = appManager().getElapsed()*0.001;
    float acc;
    for(acc = mTimeStep; acc < elapsed+ mAccomulated; acc += mTimeStep)
    {
        mWorld->Step(mTimeStep, mVelocityIterations, mPositionIterations);
        mWorld->ClearForces();
    }
    mAccomulated = acc - (elapsed + mAccomulated);
}

