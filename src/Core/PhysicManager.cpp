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

void PhysicManager::step()
{
    mWorld->Step(mTimeStep, mVelocityIterations, mPositionIterations);
    mWorld->ClearForces();
}

