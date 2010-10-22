#ifndef _PHYSIC_MANAGER_HPP_
#define _PHYSIC_MANAGER_HPP_

#include <boost/serialization/singleton.hpp>
#include <Box2D/Box2D.h>

#include <list>

#include "Physic/Body.hpp"
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
        std::list<Body>* mBodies;
    public:
        float32 mTimeStep;
        int32 mVelocityIterations;
        int32 mPositionIterations;


        PhysicManager();
        ~PhysicManager();

        b2World& getWorld();
        void registerBody(Body* body);
        std::list<Body>& getBodies();
        void step();
        void updateVisual();
};

#endif

