#include "Physic/Body.hpp"



Body::Body(b2Body* body)
{
    mBody = body;
    body->SetUserData(this);
    mAppliedImpacts = new list<Impact*>;
    mShouldFreeBodyVisual = true;
    mIsDefaultMaterial = true;
    mSouldFreeBodyMaterial =false;
    mShouldFreeB2Body = true;
    mParentWorld = mBody->GetWorld();
    mFixtureCount = 0;
    mState = new BodyState;
}

Body::~Body()
{
    delete mAppliedImpacts;
    delete mState;
    if(mShouldFreeBodyVisual) if(mBodyVisual != NULL) delete mBodyVisual;
    if(mSouldFreeBodyMaterial) if(mState->Material != NULL) delete mState->Material;
    if(mShouldFreeB2Body) mParentWorld->DestroyBody(mBody);
}

BodyState* Body::getState()
{
    return mState;
}

b2Fixture* Body::createFixture(b2FixtureDef *def)
{

    b2Fixture *fixture = mBody->CreateFixture(def);
    mFixtureCount++;
    return fixture;
}

void Body::setVisual(BodyVisual* visualiser)
{
    mBodyVisual = visualiser;
    if(visualiser != NULL)
    {
        visualiser->setBodyState(mState);
    }

}

b2Body& Body::getb2Body()
{
    return *mBody;
}

void Body::setMaterial(BodyMaterial *material)
{
    mState->Material = material;
    mSouldFreeBodyMaterial = true;
}

BodyMaterial* Body::getMaterial()
{
    return mState->Material;
}

void Body::updateVisual()
{
    if(mBodyVisual != NULL)
    {
        mBodyVisual->redrawBody();
    }
}

void Body::applyImpact(Impact* impact)
{
    mAppliedImpacts->push_front(impact);
}

void Body::cancelImpact(Impact* impact)
{
    mAppliedImpacts->remove(impact);
}


void Body::step(float32 timeStep)
{
    calculateInfluences(timeStep);

    for(std::list<Impact*>::iterator impact = mAppliedImpacts->begin();
      impact != mAppliedImpacts->end(); ++impact)
    {
        switch((*impact)->Type)
        {
            case Moisten:
                calculateMoistenImpact(*impact, timeStep);
            break;
            case Heat:
                calculateHeatImpact(*impact, timeStep);
            break;
            case Cool:
                calculateCoolImpact(*impact, timeStep);
            break;
            case Beat:
                calculateBeatImpact(*impact, timeStep);
            break;
            case Wind:
                calculateWindImpact(*impact, timeStep);
            break;
            case Electricity:
                calculateElectricityImpact(*impact, timeStep);
            break;
            case Nothing: break;
        }
    }


}

void Body::calculateInfluences(float32 timeStep)
{
    if(mState->KindleLevel > 0)
    {
        mState->CarbonizeLevel += timeStep * mState->Material->SelfFlareUpRate;
        if( mState->CarbonizeLevel > 1) mState->CarbonizeLevel = 1;
    }
    else
    {
        mState->Temperature -= (worldManager.mEnvironTemperature - mState->Temperature)/2 *
            mState->Material->ThermalReceptivity * timeStep;
    }

    if(mState->Dampness > 0)
    {
        mState->Dampness -= timeStep * mState->Temperature * mState->Material->InflTemperatureToDampness;

        b2Fixture* fixture = mBody->GetFixtureList();
        if(fixture != NULL)
        {
            float friction = fixture->GetFriction() - mState->Dampness * mState->Material->InflDampnessToFriction;
            fixture->SetFriction(friction);
        }
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

void Body::calculateMoistenImpact(Impact* impact, float32 timeStep)
{
    mState->Dampness += impact->Intensity * timeStep * mState->Material->DampReceptivity;
    if(mState->Dampness > mCurrentMaxDumpness)
    {
        mState->Dampness = mCurrentMaxDumpness;
    }
    if( mState->KindleLevel > 0)
    {
        mState->KindleLevel -= impact->Intensity * timeStep * mState->Material->InflMoistenToKindleLevel;
    }
}
 void Body::calculateHeatImpact(Impact* impact, float32 timeStep)
{
    if(mState->KindleLevel == 0)
    {
        mState->Temperature += impact->Intensity * timeStep *
            mState->Material->ThermalReceptivity;
    }
    // If body burning
    if(mState->Temperature >= mCurrentKindleTemperature)
    {
        if( mCurrentKindleTemperature > 0)
        {
            // Up flame size
            mState->KindleLevel += impact->Intensity *
                timeStep * mCurrentKindleReceptivity;
            if(mState->KindleLevel > mCurrentMaxKindle)
            {
                mState->KindleLevel = mCurrentMaxKindle;
            }
            // Up carbonize level and temperature
            if( mCurrentMaxKindle > 0)
            {
                mState->CarbonizeLevel += (mState->KindleLevel / mCurrentMaxKindle) *
                    mState->Material->CarbonizeRate * timeStep;
                if(mState->CarbonizeLevel > 1) mState->CarbonizeLevel = 1;

                mState->Temperature = mState->Material->FlameTemperature *
                    mState->KindleLevel / mCurrentMaxKindle;
            }
        }
    }
}
 void Body::calculateCoolImpact(Impact* impact, float32 timeStep)
{
    if(mState->KindleLevel == 0)
    {
        mState->Temperature -= impact->Intensity * timeStep *
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
 void Body::calculateBeatImpact(Impact* impact, float32 timeStep)
{
    b2Vec2 direction = impact->Dirrection;
    direction.Normalize();
    b2Vec2 impulsVec = impact->Intensity * direction;
    mBody->ApplyLinearImpulse(impulsVec, impact->ImpactPoint);
}
 void Body::calculateWindImpact(Impact* impact, float32 timeStep)
{

}
 void Body::calculateElectricityImpact(Impact* impact, float32 timeStep)
{

}
