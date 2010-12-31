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

#include "Physic/PhysicWind.hpp"
#include "Physic/Impact.hpp"
#include "Physic/BodyPart.hpp"

void PhysicWind::init(float power, b2Vec2 dirrection)
{
    Impact* mImpact = new Impact(Wind);
    mImpact->Intensity = power;
    mImpact->Dirrection = dirrection;
    mWeakerImpactsCount = 10;
    mWeakerImpacts = new Impact*[mWeakerImpactsCount];
    for(int i=0; i<mWeakerImpactsCount; i++)
    {
        mWeakerImpacts[i] = new Impact(Wind);
    }
    calculateWeakerImpacts();
    mRegionUpdatedSlot = areaManager().mRegionUpdated.connect(this, &PhysicWind::regionUpdated);
    regionUpdated(areaManager().getCellRegion());
}

 PhysicWind::~PhysicWind()
 {
     mRegionUpdatedSlot.destroy();
     delete mImpact;
     for(int i=0; i<mWeakerImpactsCount; i++)
     {
        delete mWeakerImpacts[i];
     }
     delete mWeakerImpacts;

 }

void PhysicWind::calculateWeakerImpacts()
{
    for(int i=0; i<mWeakerImpactsCount; i++)
    {
        mWeakerImpacts[i]->Intensity = mImpact->Intensity / ((mWeakerImpactsCount - i) * 10);
        mWeakerImpacts[i]->Dirrection = mImpact->Dirrection;
    }
}

void PhysicWind::regionUpdated(CellRegion region)
{
    CellRegion allRegion = areaManager().getCellRegion();

    // Scan by horizontal lines

    // Choose dirrection of scan
    bool normalDirrection = (mImpact->Dirrection.x > 0);
    int from = 0;
    int to =0;
    int delta = 0;
    if(normalDirrection)
    {
        from = region.lowerX;
        to = allRegion.upperX;
        delta = 1;
    }
    else
    {
        from = region.upperX;
        to = allRegion.lowerX;
        delta = -1;
    }

    int weak = -1; // -1 - standart wind power, -1 < weak <mWeakerImpactCount - weaked
    bool afterBody = false; // Are we at area of weaked wind after body
    for(int y=region.lowerY; y<=region.upperY; ++y)
    {
        weak = getWeakness(areaManager().getWindImpact(region.lowerX, y));
        for(int x=from; x!=to; x+=delta)
        {
            Impact* impact;
            if(weak < 0) impact = mImpact;
            else impact = mWeakerImpacts[weak];
            areaManager().setWindImpact(impact, x, y);

            b2Fixture* cellb2Fixture = areaManager().getCellFixture(x,y);
            if(cellb2Fixture != NULL)
            {
                BodyPart* body = reinterpret_cast<BodyPart*>(cellb2Fixture->GetUserData());
                body->applyWindImpact(impact);
                afterBody = true;
            }
            else
            {
                if(afterBody)
                {
                    if(weak >= mWeakerImpactsCount)
                    {
                        weak = -1;
                        afterBody = false;
                    }
                    else
                    {
                        weak++;
                    }
                }
            }
        }
    }
}

int PhysicWind::getWeakness(Impact* impact)
{
    if(impact == NULL) return -1;
    if(impact == mImpact) return -1;
    for(int i=0; i<mWeakerImpactsCount; i++)
    {
        if(impact == mWeakerImpacts[i]) return i;
    }
    return -1;
}

void PhysicWind::setDirrection(b2Vec2 value)
{
    mImpact->Dirrection = value;
    calculateWeakerImpacts();
}

void PhysicWind::setPower(float value)
{
    mImpact->Intensity = value;
    calculateWeakerImpacts();
}
