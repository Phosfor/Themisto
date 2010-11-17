#ifndef _DEBUG_WATCHER_HPP_
#define _DEBUG_WATCHER_HPP_

#include <boost/serialization/singleton.hpp>
#include <vector>
#include <list>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <ClanLib/core.h>
#include <sstream>


#include "Physic/Body.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"

#include "Core/PhysicManager.hpp"

using namespace std;

#define debugWatcher (DebugWatcher::get_mutable_instance())
#define debugWatcherConst (DebugWatcher::get_const_instance())

#define DEFAULT_TIMEOUT 500;

enum WatchType
{
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
    string OutFile;
    Target Object;
    string MemberName;
    boost::function<string (Watch*)> Expression;
    Watch* Parent;
    list<Watch*> Children;  
    
    Watch();  
};

class DebugWatcher: public
boost::serialization::singleton<DebugWatcher>
{
    private:
        list<Watch*> mWatches;
        CL_Timer mTimer;
        int mTimeout; // Mileseconds, if less then zero, then every step

        void parseCommand(string command);
        void process_hide(vector<string>& commandSet);
        void process_stop(vector<string>& commandSet);
        map<Target, string>& getTargets(vector<string> &commandSet, TargetType type);
        void process_material(Watch* watch, vector<string> &commandSet);
        b2Fixture* getFixture(Body* body, string partID);
        void addWatchCommon(Watch* watch, vector<string> &commandSet);
        //string evalute_material(Watch* watch);
        //string floatToStr(float p);
        Watch* getWatchByID(string id);
        void update();
        void addWatch(Watch* watch, vector<string> commandSet);
    public:
        void init();
        void step();
};



#endif
