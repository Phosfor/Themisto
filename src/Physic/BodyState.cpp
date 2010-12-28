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

#include "Physic/BodyState.hpp"

BodyState::BodyState()
{
    // How big flame dance on body
    // 0 - no flame, maxKindleLevel - flare for full stench
    KindleLevel = 0;

     // 0 - body ok, 1 - burned to dust
    CarbonizeLevel = 0;

    // Temperature in Kelvins
    Temperature = worldManager().mEnvironTemperature;

    // How many water in body
    // 0 - dry, maxDampness - full of water
    Dampness = 0;

    IsFrozen = false;
}
