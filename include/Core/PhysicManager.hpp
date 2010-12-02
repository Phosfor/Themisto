/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _PHYSIC_MANAGER_HPP_
#define _PHYSIC_MANAGER_HPP_

#include <boost/serialization/singleton.hpp>
#include <Box2D/Box2D.h>

#include <list>

#include "World/Objects/Body.hpp"
#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

#define physicManager (PhysicManager::get_mutable_instance())
#define physicManagerConst (PhysicManager::get_const_instance())

class Body;

class PhysicManager : public
boost::serialization::singleton<PhysicManager>
{
    private:
        b2World* mWorld;

    public:
        float32 mTimeStep;
        int32 mVelocityIterations;
        int32 mPositionIterations;

        PhysicManager();
        ~PhysicManager();

        b2World& getWorld();
        std::list<Body*> getBodies();
        void step();
        void disposeScene();
};

#endif

