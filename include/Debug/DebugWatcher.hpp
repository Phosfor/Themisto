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
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <fstream>

#include "Physic/Body.hpp"

#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"
#include "Core/Utils.hpp"
#include "Debug/DebugIO.hpp"
#include "Debug/EvaluteFunctions.hpp"
#include "Debug/Watch.hpp"

#include "Core/PhysicManager.hpp"

using namespace std;
using namespace evalute;
using namespace boost::lambda;

#define debugWatcher (DebugWatcher::get_mutable_instance())
#define debugWatcherConst (DebugWatcher::get_const_instance())




typedef vector<string>::iterator StrIterator;


class DebugWatcher: public
boost::serialization::singleton<DebugWatcher>
{
    private:
        list<Watch*> mWatches;
        int mTimeout; // Mileseconds, if less then zero, then every step
        int mLeftFromLastUpdate;
        ApplicationManager* _appManager;
        CL_Slot mCommandSlot;
        std::map<string, ofstream*> mFiles;
        std::map<ofstream*, int> mFilesUsing;
        
        string assignWatchToFile(Watch* watch, string file);
        string assignWatchToFile(Watch* watch, ofstream* file);
        string unassignWatchFromFile(Watch* watch);

        vector<Watch*> getWatches(StrIterator specIt, StrIterator endIt, string& answer);
        int processEvery(StrIterator everyIt, StrIterator end, string& answer);
        string process_hide(StrIterator commandIt, StrIterator endIt);
        string process_stop(StrIterator commandIt, StrIterator endIt);
        map<Target, string> getTargets(StrIterator command, StrIterator end, TargetType type, string& answer);
        string process_material(Watch* watch, vector<string> &commandSet);
        b2Fixture* getFixture(Body* body, string* partID);
        string addWatchCommon(Watch* watch, vector<string> &commandSet);
        string add_member_watch(Watch* watch, string command, 
            const string members[], const int memberCount, map<Target, string>& targets, EvaluteFunction evalute);

        Watch* getWatchByID(string id);
        void update(Watch* watch);
        void addWatchToConsole(Watch* watch);
        void updateWatchInConsole(Watch* watch);
        void removeWatchFromConsole(Watch* watch);

    public:
        void parseCommand(string command, string* answer);
        void init();
        void step();
        ~DebugWatcher();
};



#endif
