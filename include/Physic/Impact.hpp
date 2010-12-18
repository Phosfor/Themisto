/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _IMPACT_HPP_
#define _IMPACT_HPP_

#include <ClanLib/core.h>
#include <Box2D/Common/b2Math.h>

enum ImpactTypes
{
    Nothing = 0,
    Moisten = 1,
    Heat = 2,
    Cool = 3,
    Wind = 5,
    Electricity = 6
};

struct Impact
{
    ImpactTypes Type;
    // Moisten: ammount of water per time unit
    // Heat, Cool: delta temperature per time unit, for body with ThermalCapacity=1
    // Beat: impuls
    // Wind: ?
    // Electicity: ?
    float Intensity;
    b2Vec2 ImpactPoint;
    b2Vec2 Dirrection;

    Impact(ImpactTypes type);
    Impact(ImpactTypes type, float intensity, b2Vec2 impactPoint, b2Vec2 dirrection);
};

#endif /* _IMPACT_HPP_ */
