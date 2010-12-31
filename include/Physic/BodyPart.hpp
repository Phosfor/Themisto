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

#ifndef _BODY_PART_HPP_
#define _BODY_PART_HPP_

#include <iostream>
#include <list>
#include <map>

#include <Box2D/Box2D.h>
#include <ClanLib/core.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/b2Collision.h>

#include "World/WorldManager.hpp"
#include "Physic/AreaManager.hpp"

class BodyMaterial;
class Impact;
class BodyState;
class BodyVisual;

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
        std::string mName;

        float mMaxKindleLevel;
        float mMaxDampness;
        bool mAcceptsCord;
        bool mIsDefaultMaterial;

        float mCurrentMaxKindle;
        float mCurrentKindleTemperature;
        float mCurrentKindleReceptivity;
        float mCurrentFrozingTemperature;
        float mCurrentMaxDampness;
        float mCurrentElectricalConductivity;

        virtual void calculateImpacts(float32 timeStep);
        virtual void calculateInfluences(float32 timeStep);
        virtual void calculateMoistenImpact(Impact* impact, float32 timeStep);
        virtual void calculateHeatImpact(Impact* impact, float32 timeStep);
        virtual void calculateCoolImpact(Impact* impact, float32 timeStep);
        virtual void calculateWindImpact(Impact* impact, float32 timeStep);
        virtual void calculateElectricityImpact(Impact* impact, float32 timeStep);

        void findStaticCollisions();
        void calculateThermalTransmissions();
        void calculateThermalTransmission(BodyPart* p);

    public:
        BodyPart(b2Fixture* fixture, BodyMaterial* material);
        virtual ~BodyPart();
        void setMaterial(BodyMaterial *material, bool isDefault);
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
        bool IsDefaultMaterial(){ return mIsDefaultMaterial;}

        // Sould boyd free memory under mMaterial object at sestroing
        // By default is false
        bool mShouldFreeBodyMaterial;

        // How big flame can store this body
        float getMaxKindleLevel() { return mMaxKindleLevel; }
        void setMaxKindleLevel(float value) { mMaxKindleLevel = value; }

        // How many water body can soak up
        float getMaxDampness(){ return mMaxDampness; }
        void setMaxDampness(float value){ mMaxDampness = value; }

        // Possible to cord up body
        bool getAcceptsCord() { return mAcceptsCord; }
        void setAcceptsCord(bool value) { mAcceptsCord = value; }

        std::string getName() { return mName; }
        void setName(const std::string &value){ mName.assign(value); }
};

#endif /* _BODY_PART_HPP_ */
