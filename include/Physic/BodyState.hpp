/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

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

#ifndef _BODY_STATE_HPP_
#define _BODY_STATE_HPP_

#include <string.h>
#include "Physic/BodyMaterial.hpp"
#include "World/WorldManager.hpp"

class BodyMaterial;
struct BodyState
{
        // Is body temperature under the ForzingTemperature
        bool IsFrozen;

        // 0 - body ok, 1 - burned to dust
        float CarbonizeLevel;

        // Temperature in Kelvins
        float Temperature;

        bool Wet;

        BodyState();
};

#endif /* _BODY_STATE_HPP_ */
