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
#include <list>
#include <string>
#include <sstream>
#include <fstream>

#include <boost/serialization/singleton.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>

#include <ClanLib/core.h>

#include <Box2D/Collision/Shapes/b2Shape.h>

#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"
#include "Core/Utils.hpp"

#include "Debug/DebugIO.hpp"
#include "Debug/EvaluteFunctions.hpp"
#include "Core/PhysicManager.hpp"

#include "Debug/Watch.hpp"

class BodyPart;
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
