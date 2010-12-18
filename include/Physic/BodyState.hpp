/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _BODY_STATE_HPP_
#define _BODY_STATE_HPP_

#include <string.h>
#include "Physic/BodyMaterial.hpp"
#include "World/WorldManager.hpp"

struct BodyState
{
        // Is body temperature under the ForzingTemperature
        bool IsFrozen;

        // How big flame dance on body
        // 0 - no flame, maxKindleLevel - flare for full stench
        float KindleLevel;

        // 0 - body ok, 1 - burned to dust
        float CarbonizeLevel;

        // Temperature in Kelvins
        float Temperature;

        // How many water in body
        // 0 - dry, maxDampness - full of water
        float Dampness;

        BodyState();
};

#endif /* _BODY_STATE_HPP_ */
