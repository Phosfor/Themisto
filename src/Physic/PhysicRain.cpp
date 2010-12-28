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

#include "Physic/PhysicRain.hpp"

void PhysicRain::init(float intencity)
{
    mImpact = new Impact(Moisten);
    mImpact->Intensity = intencity;

    mRegionUpdatedSlot = areaManager().mRegionUpdated.connect(this, &PhysicRain::regionUpdated);
    regionUpdated(areaManager().getCellRegion());
}

PhysicRain::~PhysicRain()
{
    mRegionUpdatedSlot.destroy();
    delete mImpact;
}

void PhysicRain::regionUpdated(CellRegion region)
{
    CellRegion allRegion = areaManager().getCellRegion();
    // Scan region by vertical parallel lines
    for(int x = region.lowerX; x <= region.upperX; ++x)
    {
        bool applied = false;
        for(int y=allRegion.upperY; y>=allRegion.lowerY; --y)
        {
            b2Fixture* cellb2Fixture = areaManager().getCellFixture(x,y);
            if(cellb2Fixture != NULL)
            {
                BodyPart* body = (BodyPart*)cellb2Fixture->GetUserData();
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
