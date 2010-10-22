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
