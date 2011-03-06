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

#ifndef _PHYSIC_MANAGER_HPP_
#define _PHYSIC_MANAGER_HPP_

#include <list>

#include <boost/serialization/singleton.hpp>
#include <Box2D/Box2D.h>
#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/LevelManager.hpp"

class PhysicObject;
class PhysicManager;
class LevelManager;
PhysicManager &physicManager();

class ContactListener: public b2ContactListener
{
    private:
        PhysicManager* mPhysicManager;
    public:
        ContactListener();
        void BeginContact (b2Contact *contact);
        void EndContact (b2Contact *contact);
        void PreSolve (b2Contact *contact, const b2Manifold *oldManifold);
        void PostSolve (b2Contact *contact, const b2ContactImpulse *impulse);
};

class PhysicManager : public boost::serialization::singleton<PhysicManager>
{
    friend class ContactListener;

    private:
        b2World* mWorld;
        float mAccomulated;
        LevelManager *mLevelManager;
        ContactListener* mContactListener;


    public:
        float32 mTimeStep;
        int32 mVelocityIterations;
        int32 mPositionIterations;

        CL_Signal_v1<b2Contact*> BeginContact;
        CL_Signal_v1<b2Contact*> EndContact;
        CL_Signal_v2<b2Contact*, const b2Manifold*> PreSolveContact;
        CL_Signal_v2<b2Contact*, const b2ContactImpulse*> PostSolveContact;


        PhysicManager();
        ~PhysicManager();

        b2World& getWorld();
        std::list<PhysicObject*> getBodies();
        void update(float elapsed);
        void disposeScene();


};

inline PhysicManager &physicManager() { return PhysicManager::get_mutable_instance(); }

#endif /* _PHYSIC_MANAGER_HPP_ */
