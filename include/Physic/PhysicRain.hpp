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

#endif
