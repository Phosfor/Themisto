/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

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

using namespace evalute;

typedef std::vector<std::string>::iterator StrIterator;
typedef std::pair<std::string, Body*> TargetInfo; // ID and parent body

class DebugWatcher: public boost::serialization::singleton<DebugWatcher>
{
    private:
        std::list<Watch*> mWatches;
        int mTimeout; // Mileseconds, if less then zero, then every step
        int mLeftFromLastUpdate;
        CL_Slot mCommandSlot;
        std::map<std::string, std::ofstream*> mFiles;
        std::map<std::ofstream*, int> mFilesUsing;

        std::string assignWatchToFile(Watch* watch, std::string file, bool rewrite);
        std::string assignWatchToFile(Watch* watch, std::ofstream* file, bool rewrite);
        std::string unassignWatchFromFile(Watch* watch, bool dispose);
        std::string getFileName(std::ofstream* file);

        std::vector<Watch*> getWatches(StrIterator specIt, StrIterator endIt, std::string& answer, bool child);
        int processEvery(StrIterator everyIt, StrIterator end, std::string& answer);
        std::string process_hide(StrIterator commandIt, StrIterator endIt);
        std::string process_remove(StrIterator commandIt, StrIterator endIt);
        std::string process_stop_resume(StrIterator commandIt, StrIterator endIt, bool stop);
        std::string process_parent(StrIterator cmdIt, StrIterator endIt, Watch* watch);
        std::map<Target, TargetInfo> getTargets(StrIterator command, StrIterator end, TargetType type, std::string& answer);
        std::string process_material(Watch* watch, std::vector<std::string> &commandSet);
        std::vector<b2Fixture*> getFixtures(Body* body, std::string* partID);
        std::string addWatchCommon(Watch* watch, std::vector<std::string> &commandSet);
        std::string add_member_watch(Watch* watch, std::string command,
            const std::string members[], const int memberCount, std::map<Target, TargetInfo>& targets, EvaluteFunction evalute);

        Watch* getWatchByID(std::string id);
        void update(Watch* watch);
        void addWatchToConsole(Watch* watch);
        void updateWatchInConsole(Watch* watch);
        void removeWatchFromConsole(Watch* watch);
        void notifyConsoleChangedParent(Watch* watch);

    public:
        void parseCommand(std::string command, std::string* answer);
        void init();
        void step();
        ~DebugWatcher();
};

inline DebugWatcher &debugWatcher() { return DebugWatcher::get_mutable_instance(); }

#endif /* _DEBUG_WATCHER_HPP_ */
