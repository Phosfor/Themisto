/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _WATCH_HPP_
#define _WATCH_HPP_

#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"
#include "Core/Utils.hpp"
#include "Physic/Body.hpp"
#include <vector>
#include <boost/variant.hpp>
#include <fstream>

struct Watch;
typedef boost::function<string (Watch*)> EvaluteFunction;

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
   Body*,
   BodyPart*,
   ApplicationManager*,
   EnvironObject*,
   void*
> Target;

enum TargetType
{
    tError = 0,
    tBody=1,
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

#endif /* _WATCH_HPP_ */
