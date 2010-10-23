#ifndef _PHYSIC_BODY_HPP_
#define _PHYSIC_BODY_HPP_

#include <Box2D/Box2D.h>
#include <ClanLib/core.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>

#include "Core/Application.hpp"
#include "Physic/BodyVisual.hpp"
#include "Physic/BodyMaterial.hpp"
#include "Physic/Impact.hpp"


class BodyVisual;

class Body
{
    private:
        b2Body *mBody;
        BodyVisual *mBodyVisual;
        std::list<Impact*> *mAppliedImpacts;
        BodyMaterial *mMaterial;
        b2World *mParentWorld;
        float mCurrentMaxKindle;
        float mCurrentKindleTemperature;
        float mCurrentKindleReceptivity;
        float mCurrentFrozingTemperature;
        float mCurrentMaxDumpness;
        float mCurrentElectricalConductivity;

    protected:
        virtual void calculateMoistenImpact(Impact* impact);
        virtual void calculateHeatImpacts(Impact* impact);
        virtual void calculateCoolImpacts(Impact* impact);
        virtual void calculateBeatImpacts(Impact* impact);
        virtual void calculateWindImpacts(Impact* impact);
        virtual void calculateElectricityImpacts(Impact* impact);

    public:
        Body(b2Body* body);
        ~Body();
        void setVisual(BodyVisual* visualiser);
        b2Body& getb2Body();
        void updateVisual();
        void step();
        virtual void applyImpact(Impact* impact);
        virtual void cancelImpact(Impact* impact);
        void setMaterial(BodyMaterial *material);
        BodyMaterial* getMaterial();

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

        // Is body temperature under the ForzingTemperature
        bool isFrozen();

        // 0 - body will sink, more - stronger force will pop body from water
        float Buoyancy;

        // How big flame dance on body
        // 0 - no flame, maxKindleLevel - flare for full stench
        float mKindleLevel;

        // How big flame can store this body
        float mMaxKindleLevel;

        // 0 - body ok, 1 - burned to dust
        float mCarbonizeLevel;

        // Temperature in Kelvins
        float mTemperature;

        // How many water in body
        // 0 - dry, maxDampness - full of water
        float mDampness;

        // How many water body can soak up
        float mMaxDampness;

        // Possible to cord up body
        bool mAcceptsCord;


};

#endif
