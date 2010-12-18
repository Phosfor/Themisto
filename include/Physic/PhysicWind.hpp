/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _PHYSIC_WIND_HPP_
#define _PHYSIC_WIND_HPP_

#include "Physic/AreaManager.hpp"
#include <Box2D/Box2D.h>
#include <ClanLib/core.h>

class PhysicWind
{
    private:
        Impact* mImpact;
        Impact** mWeakerImpacts;
        int mWeakerImpactsCount;
        void calculateWeakerImpacts();
        void regionUpdated(CellRegion region);
        CL_Slot mRegionUpdatedSlot;
        int getWeakness(Impact* impact);

    public:
        void setPower(float value);
        void setDirrection(b2Vec2 value);
        void init(float power, b2Vec2 dirrection);
        ~PhysicWind();
};

#endif /* _PHYSIC_WIND_HPP_ */
