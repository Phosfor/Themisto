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
#include <iostream>
#include <list>

class Body
{
    private:
        BodyVisual *mBodyVisual;

    protected:

        b2Body *mBody;
        BodyState* mState;
        std::list<Impact*> *mAppliedImpacts;
        b2World *mParentWorld;

        float mCurrentMaxKindle;
        float mCurrentKindleTemperature;
        float mCurrentKindleReceptivity;
        float mCurrentFrozingTemperature;
        float mCurrentMaxDumpness;
        float mCurrentElectricalConductivity;

        virtual void calculateInfluences(float32 timeStep);
        virtual void calculateMoistenImpact(Impact* impact, float32 timeStep);
        virtual void calculateHeatImpact(Impact* impact, float32 timeStep);
        virtual void calculateCoolImpact(Impact* impact, float32 timeStep);
        virtual void calculateBeatImpact(Impact* impact, float32 timeStep);
        virtual void calculateWindImpact(Impact* impact, float32 timeStep);
        virtual void calculateElectricityImpact(Impact* impact, float32 timeStep);


    public:
        Body(b2Body* body);
        ~Body();

        void setVisual(BodyVisual* visualiser);
        void setMaterial(BodyMaterial *material);
        BodyState *getState();
        BodyMaterial* getMaterial();
        b2Body& getb2Body();

        void updateVisual();
        void step(float32 timeStep);

        virtual void applyImpact(Impact* impact);
        virtual void cancelImpact(Impact* impact);


        b2Fixture* createFixture(b2FixtureDef *def);

        int mFixtureCount;

        // Applied material to body or not
        // Notice, that defaut material is one object for all
        bool mIsDefaultMaterial;

        // Sould boyd free memory under mMaterial object at sestroing
        // By default is false, but setted to true after some material applied
        bool mSouldFreeBodyMaterial;

        // Should body free memory under mBodyVisual object at destroing
        // Default value is true
        bool mShouldFreeBodyVisual;

        // Should body free memory under mBody object at destroing
        // Default is true
        bool mShouldFreeB2Body;

        // How big flame can store this body
        float mMaxKindleLevel;

        // 0 - body will sink, more - stronger force will pop body from water
        float Buoyancy;

        // How many water body can soak up
        float mMaxDampness;

        // Possible to cord up body
        bool mAcceptsCord;

};


#endif
