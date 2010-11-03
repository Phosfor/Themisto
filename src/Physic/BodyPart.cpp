#include "Physic/BodyPart.hpp"


BodyPart::BodyPart(b2Fixture* fixture)
{
    mFixture = fixture;

    fixture->SetUserData(this);
    mAppliedImpacts = new list<Impact*>;
    mIsDefaultMaterial = true;
    mSouldFreeBodyMaterial =false;
    mParentWorld= mFixture->GetBody()->GetWorld();
    mState = new BodyState;
    world= &worldManager;
    mStaticCollisions = new list<BodyPart*>;
    mContactImpacts = new std::map<b2Fixture*, Impact*>;
    findStaticCollisions();
}
BodyPart::~BodyPart()
{
    delete mAppliedImpacts;
    delete mState;
    delete mStaticCollisions;
    if(mSouldFreeBodyMaterial) if(mState->Material != NULL) delete mState->Material;
    for(std::map<b2Fixture*, Impact*>::iterator i = mContactImpacts->begin();
            i != mContactImpacts->end(); ++i)
    {
        delete (*i).second;
    }
    delete mContactImpacts;
}

void BodyPart::findStaticCollisions()
{
    b2Transform t;
    for(b2Fixture* fixture = mFixture->GetNext();
            fixture != NULL; fixture = fixture->GetNext())
    {
        if(b2TestOverlap(mFixture->GetShape(), fixture->GetShape(), t, t))
        {
            mStaticCollisions->push_front((BodyPart*)fixture->GetUserData());
        }
    }
}

void BodyPart::applyRainImpact(Impact *impact)
{
    mRainImpact = impact;
}
void BodyPart::applyWindImpact(Impact *impact)
{
    mWindImpact = impact;
}

BodyState* BodyPart::getState()
{
    return mState;
}


b2Fixture* BodyPart::getFixture()
{
    return mFixture;
}

void BodyPart::setMaterial(BodyMaterial *material)
{
    mState->Material = material;
    mSouldFreeBodyMaterial = true;
}

BodyMaterial* BodyPart::getMaterial()
{
    return mState->Material;
}

void BodyPart::applyImpact(Impact* impact)
{
    mAppliedImpacts->push_front(impact);
}

void BodyPart::cancelImpact(Impact* impact)
{
    mAppliedImpacts->remove(impact);
}


void BodyPart::step(float32 elapsed)
{
    calculateInfluences(elapsed);
    calculateImpacts(elapsed);
    calculateThermalTransmissions();
}

void BodyPart::calculateThermalTransmissions()
{
    // Between body parts
   for(std::list<BodyPart*>::iterator it = mStaticCollisions->begin();
           it != mStaticCollisions->end(); ++it)
   {
       calculateThermalTransmission(*it);
   }

   // Between other fixtures
   // Reset all impacts
   for(std::map<b2Fixture*, Impact*>::iterator it = mContactImpacts->begin();
           it != mContactImpacts->end(); ++it)
   {
       it->second->Intensity = 0;
   }
   // Calculate all contact thermal transmissions
   for(b2ContactEdge* contact = mFixture->GetBody()->GetContactList();
           contact != NULL; contact = contact->next)
   {
       if(contact->contact->GetFixtureA() == mFixture )
       {
           calculateThermalTransmission((BodyPart*)contact->contact->GetFixtureB()->GetUserData());
       }
       else if(contact->contact->GetFixtureB() == mFixture)
       {
           calculateThermalTransmission((BodyPart*)contact->contact->GetFixtureA()->GetUserData());
       }
   }
   // Reset impacts for contacts, that no more exists
   for(std::map<b2Fixture*, Impact*>::iterator it = mContactImpacts->begin(); 
           it != mContactImpacts->end(); ++it)
   {
       if(it->second->Intensity == 0)
       {
           ((BodyPart*)it->first->GetUserData())->cancelImpact(it->second);
           delete it->second;
           mContactImpacts->erase(it);
       }
   }
}

   
void BodyPart::calculateThermalTransmission(BodyPart* p)
{
      Impact* impact;
      std::map<b2Fixture*, Impact*>::iterator el = mContactImpacts->find(p->mFixture);

      if(el != mContactImpacts->end())
      {
          impact = el->second;
      }
      else
      {
          ImpactTypes type = (p->mState->Temperature > mState->Temperature)? Heat : Cool;
          impact = new Impact(type);
          mContactImpacts->insert(pair<b2Fixture*, Impact*>(p->mFixture,impact));
          p->applyImpact(impact);
      }
      impact->Intensity = abs(p->mState->Temperature - mState->Temperature);

}
void BodyPart::calculateImpacts(float32 elapsed)
{
    for(std::list<Impact*>::iterator impact = mAppliedImpacts->begin();
      impact != mAppliedImpacts->end(); ++impact)
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
            case Beat:
                calculateBeatImpact(*impact, elapsed);
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
        mState->CarbonizeLevel += elapsed * mState->Material->SelfFlareUpRate;
        if( mState->CarbonizeLevel > 1) mState->CarbonizeLevel = 1;
    }
    else
    {
        mState->Temperature -= (world->mEnvironTemperature - mState->Temperature)/2 *
            mState->Material->ThermalReceptivity * elapsed;
    }

    if(mState->Dampness > 0)
    {
        mState->Dampness -= elapsed * mState->Temperature * mState->Material->InflTemperatureToDampness;

            float friction = mFixture->GetFriction() - 
                mState->Dampness * mState->Material->InflDampnessToFriction;
            mFixture->SetFriction(friction);
    }

     // Calculate influences one time here for productivity reasons
    mCurrentMaxKindle = mMaxKindleLevel -
        mState->Dampness * mState->Material->InflDampnessToMaxKindle -
        mState->CarbonizeLevel * mState->Material->InflCarbonizeLevelToMaxKindle;
    mCurrentKindleTemperature = mState->Material->KindleTemperature -
        mState->Dampness * mState->Material->InflDampnessToKindleTemperature;
    mCurrentKindleReceptivity = mState->Material->KindleReceptivity -
        mState->Dampness * mState->Material->InflDampnessToKindleReceptivity;
    mCurrentFrozingTemperature = mState->Material->FrozingTemperature +
        mState->Dampness * mState->Material->InflDampnessToFrozingTemperature;
    mCurrentMaxDumpness = mMaxDampness - mState->CarbonizeLevel * mState->Material->InflCarbonizeLevelToMaxDampness;
    mCurrentElectricalConductivity = mState->Material->ElectricalConductivity -
        mState->CarbonizeLevel * mState->Material->InflCarbonizeLevelToElecticalConductivity;
}

void BodyPart::calculateMoistenImpact(Impact* impact, float32 elapsed)
{
    mState->Dampness += impact->Intensity * elapsed * mState->Material->DampReceptivity;
    if(mState->Dampness > mCurrentMaxDumpness)
    {
        mState->Dampness = mCurrentMaxDumpness;
    }
    if( mState->KindleLevel > 0)
    {
        mState->KindleLevel -= impact->Intensity * elapsed * mState->Material->InflMoistenToKindleLevel;
    }
}
 void BodyPart::calculateHeatImpact(Impact* impact, float32 elapsed)
{
    if(mState->KindleLevel == 0)
    {
        mState->Temperature += impact->Intensity * elapsed *
            mState->Material->ThermalReceptivity;
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
                    mState->Material->CarbonizeRate * elapsed;
                if(mState->CarbonizeLevel > 1) mState->CarbonizeLevel = 1;

                mState->Temperature = mState->Material->FlameTemperature *
                    mState->KindleLevel / mCurrentMaxKindle;
            }
        }
    }
}
 void BodyPart::calculateCoolImpact(Impact* impact, float32 elapsed)
{
    if(mState->KindleLevel == 0)
    {
        mState->Temperature -= impact->Intensity * elapsed *
            mState->Material->ThermalReceptivity;
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
 void BodyPart::calculateBeatImpact(Impact* impact, float32 elapsed)
{
    b2Vec2 direction = impact->Dirrection;
    direction.Normalize();
    b2Vec2 impulsVec = impact->Intensity * direction;
    mFixture->GetBody()->ApplyLinearImpulse(impulsVec, impact->ImpactPoint);
}
 void BodyPart::calculateWindImpact(Impact* impact, float32 elapsed)
{

}
 void BodyPart::calculateElectricityImpact(Impact* impact, float32 elapsed)
{

}

