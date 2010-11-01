#include "Physic/PhysicRain.hpp"

void PhysicRain::init(float intencity)
{
    mImpact = new Impact(Moisten);
    mImpact->Intensity = intencity;

    mRegionUpdatedSlot = areaManager.mRegionUpdated.connect(this, &PhysicRain::regionUpdated);
    regionUpdated(areaManager.getCellRegion());
}

PhysicRain::~PhysicRain()
{
    mRegionUpdatedSlot.destroy();
    delete mImpact;

}

void PhysicRain::regionUpdated(CellRegion region)
{
    CellRegion allRegion = areaManager.getCellRegion();
    // Scan region by vertical parallel lines
    for(int x = region.lowerX; x <= region.upperX; ++x)
    {
        bool applied = false;
        for(int y=allRegion.upperY; y>=allRegion.lowerY; --y)
        {
            b2Body* cellb2Body = areaManager.getCellBody(x,y);
            if(cellb2Body != NULL)
            {
                Body* body = (Body*)cellb2Body->GetUserData();
                if(applied) body->applyRainImpact(NULL);
                else
                {
                    body->applyRainImpact(mImpact);
                    applied = true;
                }
            }
        }
    }
}

void PhysicRain::setIntensity(float value)
{
    mImpact->Intensity = value;
}
