#include "Physic/Body.hpp"



Body::Body(b2Body* body)
{
    mBody = body;
    body->SetUserData(this);
    mAppliedImpacts = new list<Impact*>;
    mMaterial = physicManager.mDefaultMaterial;
    mShouldFreeBodyVisual = true;
    mIsDefaultMaterial = true;
    mSouldFreeBodyMaterial =false;
    mShouldFreeB2Body = true;
    mParentWorld = &physicManager.getWorld();
}

Body::~Body()
{
    delete mAppliedImpacts;
    if(mShouldFreeBodyVisual) if(mBodyVisual != NULL) delete mBodyVisual;
    if(mSouldFreeBodyMaterial) if(mMaterial != NULL) delete mMaterial;
    if(mShouldFreeB2Body) mParentWorld->DestroyBody(mBody);
}

void Body::setVisual(BodyVisual* visualiser)
{
    mBodyVisual = visualiser;
}

b2Body& Body::getb2Body()
{
    return *mBody;
}

void Body::setMaterial(BodyMaterial *material)
{
    mMaterial = material;
    mSouldFreeBodyMaterial = true;
}

BodyMaterial* Body::getMaterial()
{
    return mMaterial;
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

bool Body::isFrozen()
{
    return mTemperature <= mCurrentFrozingTemperature;
}

void Body::step()
{
    if(mKindleLevel > 0)
    {
        mCarbonizeLevel += physicManager.mTimeStep * mMaterial->SelfFlareUpRate;
        if( mCarbonizeLevel > 1) mCarbonizeLevel = 1;
    }
    else
    {
        mTemperature -= (physicManager.mEnvironTemperature - mTemperature)/2 *
            mMaterial->ThermalReceptivity * physicManager.mTimeStep;
    }

    if(mDampness > 0)
    {
        mDampness -= physicManager.mTimeStep * mTemperature * mMaterial->InflTemperatureToDampness;

        b2Fixture* fixture = mBody->GetFixtureList();
        if(fixture != NULL)
        {
            float friction = fixture->GetFriction() - mDampness * mMaterial->InflDampnessToFriction;
            fixture->SetFriction(friction);
        }
    }

    // Calculate influences one time here for productivity reasons
    mCurrentMaxKindle = mMaxKindleLevel -
        mDampness * mMaterial->InflDampnessToMaxKindle -
        mCarbonizeLevel * mMaterial->InflCarbonizeLevelToMaxKindle;
    mCurrentKindleTemperature = mMaterial->KindleTemperature -
        mDampness * mMaterial->InflDampnessToKindleTemperature;
    mCurrentKindleReceptivity = mMaterial->KindleReceptivity -
        mDampness * mMaterial->InflDampnessToKindleReceptivity;
    mCurrentFrozingTemperature = mMaterial->FrozingTemperature +
        mDampness * mMaterial->InflDampnessToFrozingTemperature;
    mCurrentMaxDumpness = mMaxDampness - mCarbonizeLevel * mMaterial->InflCarbonizeLevelToMaxDampness;
    mCurrentElectricalConductivity = mMaterial->ElectricalConductivity -
        mCarbonizeLevel * mMaterial->InflCarbonizeLevelToElecticalConductivity;

    for(std::list<Impact*>::iterator impact = mAppliedImpacts->begin();
      impact != mAppliedImpacts->end(); ++impact)
    {
        switch((*impact)->Type)
        {
            case Moisten:
                calculateMoistenImpact(*impact);
            break;
            case Heat:
                calculateHeatImpacts(*impact);
            break;
            case Cool:
                calculateCoolImpacts(*impact);
            break;
            case Beat:
                calculateBeatImpacts(*impact);
            break;
            case Wind:
                calculateWindImpacts(*impact);
            break;
            case Electricity:
                calculateElectricityImpacts(*impact);
            break;
            case Nothing: break;
        }
    }


}

 void Body::calculateMoistenImpact(Impact* impact)
{
    mDampness += impact->Intensity * physicManager.mTimeStep * mMaterial->DampReceptivity;
    if(mDampness > mCurrentMaxDumpness)
    {
        mDampness = mCurrentMaxDumpness;
    }
    if( mKindleLevel > 0)
    {
        mKindleLevel -= impact->Intensity * physicManager.mTimeStep * mMaterial->InflMoistenToKindleLevel;
    }
}
 void Body::calculateHeatImpacts(Impact* impact)
{
    if(mKindleLevel == 0)
    {
        mTemperature += impact->Intensity * physicManager.mTimeStep * mMaterial->ThermalReceptivity;
    }
    // If body burning
    if(mTemperature >= mCurrentKindleTemperature)
    {
        if( mCurrentKindleTemperature > 0)
        {
            // Up flame size
            mKindleLevel += impact->Intensity *
                physicManager.mTimeStep * mCurrentKindleReceptivity;
            if(mKindleLevel > mCurrentMaxKindle)
            {
                mKindleLevel = mCurrentMaxKindle;
            }
            // Up carbonize level and temperature
            if( mCurrentMaxKindle > 0)
            {
                mCarbonizeLevel += (mKindleLevel / mCurrentMaxKindle) *
                    mMaterial->CarbonizeRate * physicManager.mTimeStep;
                if(mCarbonizeLevel > 1) mCarbonizeLevel = 1;

                mTemperature = mMaterial->FlameTemperature * mKindleLevel / mCurrentMaxKindle;
            }
        }
    }
}
 void Body::calculateCoolImpacts(Impact* impact)
{
    if(mKindleLevel == 0)
    {
        mTemperature -= impact->Intensity * physicManager.mTimeStep * mMaterial->ThermalReceptivity;
    }
}
 void Body::calculateBeatImpacts(Impact* impact)
{
    b2Vec2 direction = impact->Dirrection;
    direction.Normalize();
    b2Vec2 impulsVec = impact->Intensity * direction;
    mBody->ApplyLinearImpulse(impulsVec, impact->ImpactPoint);
}
 void Body::calculateWindImpacts(Impact* impact)
{

}
 void Body::calculateElectricityImpacts(Impact* impact)
{

}
