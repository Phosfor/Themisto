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

#include "Physic/BodyPart.hpp"



BodyPart::BodyPart(b2Fixture* fixture, boost::shared_ptr<BodyMaterial> material)
{
    mFixture = fixture;
    fixture->SetUserData(this);
    mMaterial = material;
    mParentWorld = mFixture->GetBody()->GetWorld();
    mState = boost::shared_ptr<BodyState>(new BodyState());
    world= &worldManager();
    //findStaticCollisions();
    mName = world->generateUniqueID();
    mMaxKindleLevel = 0;
    mMaxDampness = 0;
    mAcceptsCord = false;
}

void BodyPart::findStaticCollisions()
{
    b2Transform t;
    for(b2Fixture* fixture = mFixture->GetNext();
            fixture != NULL; fixture = fixture->GetNext())
    {
        if(b2TestOverlap(mFixture->GetShape(), fixture->GetShape(), t, t))
        {
            mStaticCollisions.push_front(reinterpret_cast<BodyPart*>(fixture->GetUserData()));
        }
    }
}

void BodyPart::applyRainImpact(boost::shared_ptr<Impact> impact)
{
    mRainImpact = impact;
}
void BodyPart::applyWindImpact(boost::shared_ptr<Impact> impact)
{
    mWindImpact = impact;
}

boost::shared_ptr<BodyState> BodyPart::getState()
{
    return mState;
}


b2Fixture* BodyPart::getFixture()
{
    return mFixture;
}

void BodyPart::setMaterial(boost::shared_ptr<BodyMaterial> material)
{
    mMaterial = material;
}

boost::shared_ptr<BodyMaterial> BodyPart::getMaterial()
{
    return mMaterial;
}

void BodyPart::applyImpact(boost::shared_ptr<Impact> impact)
{
    mAppliedImpacts.push_front(impact);
}

void BodyPart::cancelImpact(boost::shared_ptr<Impact> impact)
{
    mAppliedImpacts.remove(impact);
}


void BodyPart::step(float32 _elapsed)
{
    float elapsed = _elapsed * 0.001; // In seconds
    calculateInfluences(elapsed);
    calculateImpacts(elapsed);
    calculateThermalTransmissions();
}

void BodyPart::calculateThermalTransmissions()
{
    // Between body parts
   for(std::list< BodyPart* >::iterator it = mStaticCollisions.begin();
           it != mStaticCollisions.end(); ++it)
   {
       calculateThermalTransmission(*it);
   }

   // Between other fixtures
   // Reset all impacts
   for(std::map<b2Fixture*, boost::shared_ptr<Impact> >::iterator it = mContactImpacts.begin();
           it != mContactImpacts.end(); ++it)
   {
       it->second->Intensity = 0;
   }
   // Calculate all contact thermal transmissions
   for(b2ContactEdge* contact = mFixture->GetBody()->GetContactList();
           contact != NULL; contact = contact->next)
   {
       if(contact->contact->GetFixtureA() == mFixture )
       {
           calculateThermalTransmission(reinterpret_cast<BodyPart*>(contact->contact->GetFixtureB()->GetUserData()));
       }
       else if(contact->contact->GetFixtureB() == mFixture)
       {
           calculateThermalTransmission(reinterpret_cast<BodyPart*>(contact->contact->GetFixtureA()->GetUserData()));
       }
   }
   // Reset impacts for contacts, that no more exists
   for(std::map<b2Fixture*, boost::shared_ptr<Impact> >::iterator it = mContactImpacts.begin();
           it != mContactImpacts.end(); ++it)
   {
       if(it->second->Intensity == 0)
       {
           (reinterpret_cast<BodyPart*>(it->first->GetUserData()))->cancelImpact(it->second);
           mContactImpacts.erase(it);
       }
   }
}

void BodyPart::calculateThermalTransmission(BodyPart* p)
{
      boost::shared_ptr<Impact> impact;
      std::map< b2Fixture*, boost::shared_ptr<Impact> >::iterator el = mContactImpacts.find(p->mFixture);

      if(el != mContactImpacts.end())
      {
          impact = el->second;
      }
      else
      {
          ImpactTypes type = (p->mState->Temperature > mState->Temperature)? Heat : Cool;
          impact = boost::shared_ptr<Impact>(new Impact(type));
          mContactImpacts.insert(std::pair< b2Fixture*, boost::shared_ptr<Impact> >(p->mFixture,impact));
          p->applyImpact(impact);
      }
      impact->Intensity = abs(p->mState->Temperature - mState->Temperature);

}
void BodyPart::calculateImpacts(float32 elapsed)
{
    for(std::list< boost::shared_ptr<Impact> >::iterator impact = mAppliedImpacts.begin();
      impact != mAppliedImpacts.end(); ++impact)
    {
        switch((*impact)->Type)
        {
            case Moisten:
                calculateMoistenImpact(*impact, elapsed);
            break;
            case Heat:
                calculateHeatImpact(*impact, elapsed);
            break;
            case Cool:
                calculateCoolImpact(*impact, elapsed);
            break;
            case Wind:
                calculateWindImpact(*impact, elapsed);
            break;
            case Electricity:
                calculateElectricityImpact(*impact, elapsed);
            break;
            case Nothing: break;
        }
    }
    if(mRainImpact != NULL)
    {
        calculateMoistenImpact(mRainImpact, elapsed);
    }
    if(mWindImpact != NULL)
    {
        calculateWindImpact(mWindImpact, elapsed);
    }

}

void BodyPart::calculateInfluences(float32 elapsed)
{
    if(mState->KindleLevel > 0)
    {
        mState->CarbonizeLevel += elapsed * mMaterial->SelfFlareUpRate;
        if( mState->CarbonizeLevel > 1) mState->CarbonizeLevel = 1;
    }
    else
    {
        mState->Temperature -= (world->mEnvironTemperature - mState->Temperature)/2 *
            mMaterial->ThermalReceptivity * elapsed;
    }

    if(mState->Dampness > 0)
    {
        mState->Dampness -= elapsed * mState->Temperature * mMaterial->InflTemperatureToDampness;

            float friction = mFixture->GetFriction() -
                mState->Dampness * mMaterial->InflDampnessToFriction;
            mFixture->SetFriction(friction);
    }

     // Calculate influences one time here for productivity reasons
    mCurrentMaxKindle = mMaxKindleLevel -
        mState->Dampness * mMaterial->InflDampnessToMaxKindle -
        mState->CarbonizeLevel * mMaterial->InflCarbonizeLevelToMaxKindle;
    mCurrentKindleTemperature = mMaterial->KindleTemperature -
        mState->Dampness * mMaterial->InflDampnessToKindleTemperature;
    mCurrentKindleReceptivity = mMaterial->KindleReceptivity -
        mState->Dampness * mMaterial->InflDampnessToKindleReceptivity;
    mCurrentFrozingTemperature = mMaterial->FrozingTemperature +
        mState->Dampness * mMaterial->InflDampnessToFrozingTemperature;
    mCurrentMaxDampness = mMaxDampness - mState->CarbonizeLevel * mMaterial->InflCarbonizeLevelToMaxDampness;
    mCurrentElectricalConductivity = mMaterial->ElectricalConductivity -
        mState->CarbonizeLevel * mMaterial->InflCarbonizeLevelToElecticalConductivity;
}

void BodyPart::calculateMoistenImpact(boost::shared_ptr<Impact> impact, float32 elapsed)
{
    mState->Dampness += impact->Intensity * elapsed * mMaterial->DampReceptivity;
    if(mState->Dampness > mCurrentMaxDampness)
    {
        mState->Dampness = mCurrentMaxDampness;
    }
    if( mState->KindleLevel > 0)
    {
        mState->KindleLevel -= impact->Intensity * elapsed * mMaterial->InflMoistenToKindleLevel;
    }
}
 void BodyPart::calculateHeatImpact(boost::shared_ptr<Impact> impact, float32 elapsed)
{
    if(mState->KindleLevel == 0)
    {
        mState->Temperature += impact->Intensity * elapsed *
            mMaterial->ThermalReceptivity;
    }
    // If body burning
    if(mState->Temperature >= mCurrentKindleTemperature)
    {
        if( mCurrentKindleTemperature > 0)
        {
            // Up flame size
            mState->KindleLevel += impact->Intensity *
                elapsed * mCurrentKindleReceptivity;
            if(mState->KindleLevel > mCurrentMaxKindle)
            {
                mState->KindleLevel = mCurrentMaxKindle;
            }
            // Up carbonize level and temperature
            if( mCurrentMaxKindle > 0)
            {
                mState->CarbonizeLevel += (mState->KindleLevel / mCurrentMaxKindle) *
                    mMaterial->CarbonizeRate * elapsed;
                if(mState->CarbonizeLevel > 1) mState->CarbonizeLevel = 1;

                mState->Temperature = mMaterial->FlameTemperature *
                    mState->KindleLevel / mCurrentMaxKindle;
            }
        }
    }
}
 void BodyPart::calculateCoolImpact(boost::shared_ptr<Impact> impact, float32 elapsed)
{
    if(mState->KindleLevel == 0)
    {
        mState->Temperature -= impact->Intensity * elapsed *
            mMaterial->ThermalReceptivity;
        if(mState->Temperature < mCurrentFrozingTemperature)
        {
            mState->IsFrozen = true;
        }
        else
        {
            mState->IsFrozen = false;
        }
    }
}
 void BodyPart::calculateWindImpact(boost::shared_ptr<Impact> impact, float32 elapsed)
{

}
 void BodyPart::calculateElectricityImpact(boost::shared_ptr<Impact> impact, float32 elapsed)
{

}
