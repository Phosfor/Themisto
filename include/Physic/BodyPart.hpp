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
#include <boost/shared_ptr.hpp>

#include "World/WorldManager.hpp"
#include "Physic/BodyMaterial.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/Impact.hpp"

class BodyMaterial;
class Impact;
class BodyState;

class BodyPart
{
    protected:
        b2Fixture* mFixture;
        std::list< boost::shared_ptr<Impact> > mAppliedImpacts;
        b2World *mParentWorld;
        boost::shared_ptr<Impact> mRainImpact;
        boost::shared_ptr<Impact> mWindImpact;
        WorldManager* world;
        std::list< BodyPart* > mStaticCollisions;
        std::map<b2Fixture*, boost::shared_ptr<Impact> > mContactImpacts;
        boost::shared_ptr<BodyMaterial> mMaterial;
        std::string mName;

        bool mIsDefaultMaterial;
        float mCurrentKindleTemperature;
        float mCurrentFrozingTemperature;

        // Is body temperature under the ForzingTemperature
        bool mIsFrozen;

        // 0 - body ok, 1 - burned to dust
        float mCarbonizeLevel;

        // Temperature in Kelvins
        float Temperature;

        bool Wet;


        virtual void calculateImpacts(float32 timeStep);
        virtual void calculateInfluences(float32 timeStep);
        virtual void calculateMoistenImpact(boost::shared_ptr<Impact> impact, float32 timeStep);
        virtual void calculateHeatImpact(boost::shared_ptr<Impact> impact, float32 timeStep);
        virtual void calculateCoolImpact(boost::shared_ptr<Impact> impact, float32 timeStep);
        virtual void calculateWindImpact(boost::shared_ptr<Impact> impact, float32 timeStep);
        virtual void calculateElectricityImpact(boost::shared_ptr<Impact> impact, float32 timeStep);

        void findStaticCollisions();
        void calculateThermalTransmissions();
        void calculateThermalTransmission(BodyPart* p);

    public:
        BodyPart(b2Fixture* fixture, boost::shared_ptr<BodyMaterial> material);
        void setMaterial(boost::shared_ptr<BodyMaterial> material);
        boost::shared_ptr<BodyState> getState();
        boost::shared_ptr<BodyMaterial> getMaterial();
        b2Fixture* getFixture();

        void step(float32 elapsed);

        virtual void applyImpact(boost::shared_ptr<Impact> impact);
        virtual void cancelImpact(boost::shared_ptr<Impact> impact);
        // Moisten impact of natural rain
        virtual void applyRainImpact(boost::shared_ptr<Impact> impact);
        // Wind impact of natural wind
        virtual void applyWindImpact(boost::shared_ptr<Impact> impact);

        std::string getName() { return mName; }
        void setName(const std::string &value){ mName.assign(value); }
};

#endif /* _BODY_PART_HPP_ */
