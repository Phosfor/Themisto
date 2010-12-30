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

#ifndef _PHYSIC_RAIN_HPP_
#define _PHYSIC_RAIN_HPP_

#include <Box2D/Box2D.h>
#include <ClanLib/core.h>

#include "Physic/AreaManager.hpp"

class Impact;
class BodyPart;
class PhysicRain
{
    private:
        Impact* mImpact;
        Impact* getWeakerImpact(float k);
        void regionUpdated(CellRegion region);
        CL_Slot mRegionUpdatedSlot;
    public:
        void setIntensity(float value);
        void init(float intencity);
        ~PhysicRain();
};

#endif /* _PHYSIC_RAIN_HPP_ */
