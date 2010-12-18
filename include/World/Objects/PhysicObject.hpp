/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */


#ifndef _PHYSIC_OBJECT_HPP_
#define _PHYSIC_OBJECT_HPP_

#include "Physic/Body.hpp"
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

class PhysicObject: public Object
{
    private:
        BodyVisual *mBodyVisual;
        Body* mBody;

    public:
        PhysicObject(Body* body, BodyVisual* visual);
        ~PhysicObject();

        // Get/set 
        void setVisual(BodyVisual* visualiser);
        BodyVisual& getVisual();
        void setBody(Body* body);
        Body& getBody();

        void updateVisual();
        void step(float32 elapsed); // Physic

        // Should body free memory under mBodyVisual object at destroing
        // Default value is true
        bool mShouldFreeBodyVisual;

        // Should body free memory under mBody object at destroing
        // Default is true
        bool mShouldFreeBody; 

        // Parsing object
        static Object* ParsePhysicObject(CL_DomElement* node, std::string &desc);

        // --- Object implementation ---
        CL_Pointf getPosition();
        void update(float elapsed);
};


#endif /* _PHYSIC_OBJECT_HPP_ */
