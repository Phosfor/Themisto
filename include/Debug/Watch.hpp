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
#include "World/Objects/Body.hpp"
#include <vector>
#include <boost/variant.hpp>
#include <fstream>

struct Watch;
typedef boost::function<string (Watch*)> EvaluteFunction;

#define DEFAULT_TIMEOUT 500
#define EVERY_STEP -1
#define AEON 65000

using namespace std;


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

string nope_evalute(Watch* watch);
   

struct Watch
{
    WatchType Type;
    string ID;
    string Name;
    bool Active;
    ofstream* OutFile;
    Target Object; // Target object
    string MemberName; // Member name of Object to show
    EvaluteFunction Expression; // Function, that evalutes watch value
    Watch* Parent;
    list<Watch*> Children;  
    int UpdateInterval; // Milliseconds; if less then zero (EVERY_STEP value) then newer update
    int LeftFromLastUpdate;
    
    Watch();
    ~Watch();  
};

#endif

