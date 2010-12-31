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

#include "Debug/Watch.hpp"
#include "Physic/Body.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/BodyPart.hpp"
#include "Physic/BodyMaterial.hpp"

std::string nope_evalute(Watch* watch)
{
    return "";
}

Watch::Watch()
{
    Active = true;
    Object = reinterpret_cast<void*>(NULL);
    MemberName = "";
    OutFile = NULL;
    Parent = NULL;
    Type = NotAWatch;
    ID = "";
    Name = "";
    Expression = nope_evalute;
    UpdateInterval = DEFAULT_TIMEOUT;
    LeftFromLastUpdate = 0;
}

Watch::~Watch()
{
}
