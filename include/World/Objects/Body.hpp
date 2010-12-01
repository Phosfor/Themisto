#ifndef _PHYSIC_BODY_HPP_
#define _PHYSIC_BODY_HPP_

#include <Box2D/Box2D.h>
#include <ClanLib/core.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>

#include "Physic/BodyMaterial.hpp"

#include "Physic/Impact.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/BodyVisual.hpp"
#include "World/WorldManager.hpp"
#include "Physic/AreaManager.hpp"
#include "World/Objects/Object.hpp"
#include "Physic/BodyPart.hpp"
#include <iostream>
#include <list>

#include "Core/PhysicManager.hpp"
#include "Core/EnvironManager.hpp"
#include "Core/ObjectsManager.hpp"
#include "World/Environ/Types.hpp"
#include "World/Objects/Types.hpp"
#include <boost/lexical_cast.hpp>

using namespace boost;

class Body: public Object
{
    private:
        BodyVisual *mBodyVisual;
        b2AABB mLastLocation;
        b2AABB calculateLocation();
        
        // 0 - body will sink, more - stronger force would pop body from water
        float mBuoyancy;

    protected:

        b2Body *mBody;
        b2World *mParentWorld;
        WorldManager* world;


    public:
        Body(b2Body* body);
        ~Body();

        // Get/set 
        void setVisual(BodyVisual* visualiser);
        b2Body* getb2Body();
        float getBuoyancy() { return mBuoyancy; }
        void setBuoyancy(float value) { mBuoyancy = value; }

        void updateVisual();
        void step(float32 elapsed);
        
        // Should body free memory under mBodyVisual object at destroing
        // Default value is true
        bool mShouldFreeBodyVisual;

        // Should body free memory under mBody object at destroing
        // Default is true
        bool mShouldFreeB2Body; 
        
        // --- Object implementation ---
        CL_Pointf getPosition();
        void update(float elapsed) { step(elapsed); }
        
        // Parsing Body object
        static void ParseBody(CL_DomElement* node);

};



#endif
