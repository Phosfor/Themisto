/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _PHYSIC_RAIN_HPP_
#define _PHYSIC_RAIN_HPP_

#include <Box2D/Box2D.h>
#include <ClanLib/core.h>
#include "Physic/AreaManager.hpp"
#include "Physic/Impact.hpp"

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
