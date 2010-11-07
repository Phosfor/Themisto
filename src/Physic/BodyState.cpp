#include "Physic/BodyState.hpp"

BodyState::BodyState()
{
    // How big flame dance on body
    // 0 - no flame, maxKindleLevel - flare for full stench
    KindleLevel = 0;

     // 0 - body ok, 1 - burned to dust
    CarbonizeLevel = 0;

    // Temperature in Kelvins
    Temperature = worldManager.mEnvironTemperature;

    // How many water in body
    // 0 - dry, maxDampness - full of water
    Dampness = 0;

    IsFrozen = false;


}

