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
}
BodyPart::~BodyPart()
{
    delete mAppliedImpacts;
    delete mState;
    if(mSouldFreeBodyMaterial) if(mState->Material != NULL) delete mState->Material;
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

