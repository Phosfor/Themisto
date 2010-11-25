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
   

struct Watch
{
    WatchType Type;
    string ID;
    string Name;
    bool Active;
    ofstream* OutFile;
    Target Object;
    string MemberName;
    EvaluteFunction Expression;
    Watch* Parent;
    list<Watch*> Children;  
    
    Watch();
    ~Watch();  
};

#endif

