/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Physic/Impact.hpp"

Impact::Impact(ImpactTypes type)
{
    Type = type;
}

Impact::Impact(ImpactTypes type, float intensity, b2Vec2 impactPoint, b2Vec2 dirrection)
{
    Type = type;
    Intensity = intensity;
    impactPoint  = impactPoint;
    Dirrection = dirrection;
}
