#ifndef _BODY_PART_HPP_
#define _BODY_PART_HPP_

#include <Box2D/Box2D.h>
#include <ClanLib/core.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/b2Collision.h>

#include "Physic/BodyMaterial.hpp"
#include "Physic/Impact.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/BodyVisual.hpp"
#include "World/WorldManager.hpp"
#include "Physic/AreaManager.hpp"
#include <iostream>
#include <list>
#include <map>


class BodyPart
{
    protected:
        b2Fixture* mFixture;
        BodyState* mState;
        std::list<Impact*> *mAppliedImpacts;
        b2World *mParentWorld;
        Impact* mRainImpact;
        Impact* mWindImpact;
        WorldManager* world;
        std::list<BodyPart*> *mStaticCollisions;
        std::map<b2Fixture*, Impact*>* mContactImpacts;
        BodyMaterial *mMaterial;

        float mCurrentMaxKindle;
        float mCurrentKindleTemperature;
        float mCurrentKindleReceptivity;
        float mCurrentFrozingTemperature;
        float mCurrentMaxDumpness;
        float mCurrentElectricalConductivity;

        virtual void calculateImpacts(float32 timeStep);
        virtual void calculateInfluences(float32 timeStep);
        virtual void calculateMoistenImpact(Impact* impact, float32 timeStep);
        virtual void calculateHeatImpact(Impact* impact, float32 timeStep);
        virtual void calculateCoolImpact(Impact* impact, float32 timeStep);
        virtual void calculateBeatImpact(Impact* impact, float32 timeStep);
        virtual void calculateWindImpact(Impact* impact, float32 timeStep);
        virtual void calculateElectricityImpact(Impact* impact, float32 timeStep);

        void findStaticCollisions();
        void calculateThermalTransmissions();
        void calculateThermalTransmission(BodyPart* p);
    public:
        BodyPart(b2Fixture* fixture, BodyMaterial* material);
        ~BodyPart();
        void setMaterial(BodyMaterial *material);
        BodyState *getState();
        BodyMaterial* getMaterial();
        b2Fixture* getFixture();

        void step(float32 elapsed);

        virtual void applyImpact(Impact* impact);
        virtual void cancelImpact(Impact* impact);
        // Moisten impact of natural rain
        virtual void applyRainImpact(Impact *impact);
        // Wind impact of natural wind
        virtual void applyWindImpact(Impact *impact);

        
        // Applied material to body or not
        // Notice, that defaut material is one object for all
        bool mIsDefaultMaterial;

        // Sould boyd free memory under mMaterial object at sestroing
        // By default is false
        bool mShouldFreeBodyMaterial;

        // How big flame can store this body
        float mMaxKindleLevel;

        // How many water body can soak up
        float mMaxDampness;

        // Possible to cord up body
        bool mAcceptsCord;



};


#endif /* _BODY_PART_HPP_ */
