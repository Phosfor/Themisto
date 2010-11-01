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
    mParentWorld= mBody->GetWorld();
    mState = new BodyState;
    mLastLocation = calculateLocation();
    world= &worldManager;
}
Body::~Body()
{
    delete mAppliedImpacts;
    delete mState;
    if(mShouldFreeBodyVisual) if(mBodyVisual != NULL) delete mBodyVisual;
    if(mSouldFreeBodyMaterial) if(mState->Material != NULL) delete mState->Material;
    if(mShouldFreeB2Body) mParentWorld->DestroyBody(mBody);
}

void Body::applyRainImpact(Impact *impact)
{
    mRainImpact = impact;
}
void Body::applyWindImpact(Impact *impact)
{
    mWindImpact = impact;
}

BodyState* Body::getState()
{
    return mState;
}

void Body::setVisual(BodyVisual* visualiser)
{
    mBodyVisual = visualiser;
    if(visualiser != NULL)
    {
        visualiser->setBodyState(mState);
    }

}

b2Body* Body::getb2Body()
{
    return mBody;
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


void Body::step(float32 elapsed)
{
    calculateInfluences(elapsed);
    calculateImpacts(elapsed);
    b2AABB currentLocation = calculateLocation();
    
    b2Vec2 ll = mLastLocation.lowerBound;
    b2Vec2 cl = currentLocation.lowerBound;
    float cellSize = areaManager.getCellSize();
    // If body moved father then cell size
    if(abs(ll.x - cl.x) > cellSize || abs(ll.y - cl.y) > cellSize)
    {
        areaManager.reportNewBodyLocation(&mLastLocation, &currentLocation, mBody);
        mLastLocation = currentLocation;
    }
}

void Body::calculateImpacts(float32 elapsed)
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

void Body::calculateInfluences(float32 elapsed)
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

        b2Fixture* fixture = mBody->GetFixtureList();
        while(fixture != NULL)
        {
            float friction = fixture->GetFriction() - mState->Dampness * mState->Material->InflDampnessToFriction;
            fixture->SetFriction(friction);
            fixture = fixture->GetNext();
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

void Body::calculateMoistenImpact(Impact* impact, float32 elapsed)
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
 void Body::calculateHeatImpact(Impact* impact, float32 elapsed)
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
 void Body::calculateCoolImpact(Impact* impact, float32 elapsed)
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
 void Body::calculateBeatImpact(Impact* impact, float32 elapsed)
{
    b2Vec2 direction = impact->Dirrection;
    direction.Normalize();
    b2Vec2 impulsVec = impact->Intensity * direction;
    mBody->ApplyLinearImpulse(impulsVec, impact->ImpactPoint);
}
 void Body::calculateWindImpact(Impact* impact, float32 elapsed)
{

}
 void Body::calculateElectricityImpact(Impact* impact, float32 elapsed)
{

}

b2AABB Body::calculateLocation()
{
    b2AABB region;
    b2Fixture* firstFixture = mBody->GetFixtureList();
    if(firstFixture != NULL)
    {
        b2AABB r = firstFixture->GetAABB();
        region.lowerBound = r.lowerBound;
        region.upperBound = r.upperBound;

        for(b2Fixture* fixture = firstFixture->GetNext(); fixture != NULL; fixture = fixture->GetNext())
        {
            region.Combine(region, fixture->GetAABB());
        }
    }
    else
    {
        region.lowerBound.Set(0,0);
        region.upperBound.Set(0,0);
    }
    return region;
}
