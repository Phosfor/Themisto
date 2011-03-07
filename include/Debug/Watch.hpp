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

#pragma once

#include <vector>
#include <fstream>

#include <boost/variant.hpp>

#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"
#include "Core/Utils.hpp"


class BodyPart;
class BodyState;
class BodyMaterial;

struct Watch;
typedef boost::function<std::string (Watch*)> EvaluteFunction;

const int DEFAULT_TIMEOUT = 500;
const int EVERY_STEP = -1;
const int AEON = 65000;

enum WatchType
{
    NotAWatch,
    ShowWatch,
    WriteWatch
};

typedef boost::variant<
   BodyMaterial*,
   BodyState*,
   b2Body*,
   b2Fixture*,
   BodyPart*,
   ApplicationManager*,
   EnvironObject*,
   void*
> Target;

enum TargetType
{
    tError = 0,
    tBodyPart=2,
    tBodyMaterial=4,
    tBodyState=8,
    tb2Body=16,
    tb2Fixture=32,
    tApplicationManager=64,
    tEnvironObject=128
};

std::string nope_evalute(Watch* watch);

struct Watch
{
    WatchType Type;
    std::string ID;
    std::string Name;
    bool Active;
    std::ofstream* OutFile;
    Target Object; // Target object
    std::string MemberName; // Member name of Object to show
    EvaluteFunction Expression; // Function, that evalutes watch value
    Watch* Parent;
    std::list<Watch*> Children;
    int UpdateInterval; // Milliseconds; if less then zero (EVERY_STEP value) then newer update
    int LeftFromLastUpdate;

    Watch();
    ~Watch();
};
