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

