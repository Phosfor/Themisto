#include "Physic/Body.hpp"



Body::Body(b2Body* body)
{
    mBody = body;
    mAppliedImpacts = new list<Impact*>;
    mMaterial = new BodyMaterial;
}

Body::~Body()
{
    delete mAppliedImpacts;
    delete mMaterial;
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
    mMaterial->Name = material->Name;
    mMaterial->KindleTemperature = material->KindleTemperature;
    mMaterial->KindleReceptivity = material->KindleReceptivity;
    mMaterial->FlameTemperature = material->FlameTemperature;
    mMaterial->SelfFlareUpRate = material->SelfFlareUpRate;
    mMaterial->CarbonizeRate = material->CarbonizeRate;
    mMaterial->ElectricalConductivity = material->ElectricalConductivity;
    mMaterial->ThermalReceptivity = material->ThermalReceptivity;
    mMaterial->DampReceptivity = material->DampReceptivity;
    mMaterial->FrozingTemperature = material->FrozingTemperature;
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
    return mTemperature <= mMaterial->FrozingTemperature;
}

void Body::step()
{
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
        }
    }
    if(mKindleLevel > 0)
    {
        mCarbonizeLevel += physicManager.mTimeStep * mMaterial->SelfFlareUpRate;
        if( mCarbonizeLevel > 1) mCarbonizeLevel = 1;
    }

}

 void Body::calculateMoistenImpact(Impact* impact)
{
    mDampness += impact->Intensity * physicManager.mTimeStep * mMaterial->DampReceptivity;
    if(mDampness > mMaxDampness)
    {
        mDampness = mMaxDampness;
    }
}
 void Body::calculateHeatImpacts(Impact* impact)
{
    if(mKindleLevel == 0)
    {
        mTemperature += impact->Intensity * physicManager.mTimeStep * mMaterial->ThermalReceptivity;
    }
    //if body burning
    if(mTemperature >= mMaterial->KindleTemperature)
    {
        if( mMaterial->KindleTemperature > 0)
        {
            //up flame size
            mKindleLevel += impact->Intensity *
                physicManager.mTimeStep * mMaterial->KindleReceptivity;
            if(mKindleLevel > mMaxKindleLevel)
            {
                mKindleLevel = mMaxKindleLevel;
            }
            //up carbonize level and temperature
            if( mMaxKindleLevel > 0)
            {
                mCarbonizeLevel += (mKindleLevel / mMaxKindleLevel) *
                    mMaterial->CarbonizeRate * physicManager.mTimeStep;
                if(mCarbonizeLevel > 1) mCarbonizeLevel = 1;

                mTemperature = mMaterial->FlameTemperature * mKindleLevel / mMaxKindleLevel;
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
