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

#include "Debug/DebugWatcher.hpp"
#include "Physic/BodyPart.hpp"

void DebugWatcher::init()
{
    //subsribe to commands
    mCommandSlot = debugIO().mCommand.connect(this, &DebugWatcher::parseCommand);

    mTimeout = DEFAULT_TIMEOUT;
    mLeftFromLastUpdate = 0;
}

DebugWatcher::~DebugWatcher()
{
    std::string answer = "";
    parseCommand("hide all", &answer);
    LOG("~DebugWatcher: " + answer);
}

void DebugWatcher::parseCommand(std::string _command, std::string* _answer)
{
    if(_command.size() == 0) return;
    std::string& answer = *_answer;
    // Normalize command
    std::string command(_command);
    boost::trim(command);
    while(command.find("  ") != command.npos)
    {
        boost::replace_all(command, "  ", " ");
    }
    while(command.find(", ") != command.npos)
    {
        boost::replace_all(command, ", ", ",");
    }
    while(command.find(" ,") != command.npos)
    {
        boost::replace_all(command, " ,", ",");
    }
    while(command.find(" )") != command.npos)
    {
        boost::replace_all(command, " )", ")");
    }
     while(command.find("( ") != command.npos)
    {
        boost::replace_all(command, "( ", "(");
    }
    LOG(command);

    std::vector<std::string> commandSet;
    boost::split(commandSet, command, boost::is_any_of(" "));
    std::string firstWord = commandSet[0];
    Watch* watch;
    if(firstWord == "show")
    {
        bool newWatch = true;
        if(commandSet.size() > 1)
        {
            if(commandSet[1] == "watch")
            {
                newWatch = false;
                if(commandSet.size() > 2)
                {
                    std::vector<Watch*> watches = getWatches(commandSet.begin() + 2, commandSet.end(),answer, true);
                    int showed = 0;
                    BOOST_FOREACH(Watch* watch, watches)
                    {
                        if(watch->Type == ShowWatch)
                        {
                            if(!watch->Active)
                            {
                                watch->Active = true;
                                addWatchToConsole(watch);
                                showed++;
                            }
                            else
                            {
                                answer += "Watch with id = '" + watch->ID + "' already showing.\n";
                            }
                        }
                        else
                        {
                            answer += "Error: can't show watch with id = '" + watch->ID + "' - it is writing watch.\n";
                        }
                    }
                    answer += "Showed '" + IntToStr(showed) +"' watches.\n";
                }
                else
                {
                    answer += "Error: watch not specified for 'show watch' command.\n";
                }
            }
        }
        if(newWatch)
        {
            watch = new Watch();
            watch->Type = ShowWatch;
            watch->Name = _command;
            answer += addWatchCommon(watch, commandSet);
        }
    }
    else if(firstWord == "hide")
    {
        answer += process_hide(commandSet.begin(), commandSet.end());
    }
    else if(firstWord == "remove")
    {
        answer += process_remove(commandSet.begin(), commandSet.end());
    }
    else if(firstWord == "update")
    {
        StrIterator argIt = commandSet.begin()+1;
        if(argIt == commandSet.end())
        {
            BOOST_FOREACH(Watch* watch, mWatches)
            {
                update(watch);
            }
            answer += "All watches updated.\n";
        }
        else
        {
            std::vector<Watch*> watches = getWatches(argIt, commandSet.end(), answer, true);
            StrIterator parIt = argIt +1;
            bool simpleUpdate = true;
            if(parIt != commandSet.end())
            {
                if(*parIt == "every")
                {
                    simpleUpdate = false;
                    int every = processEvery(argIt +1, commandSet.end(), answer);
                    BOOST_FOREACH(Watch* watch, watches)
                    {
                        watch->UpdateInterval = every;
                    }
                    answer += "Update interval setted for "+ IntToStr(watches.size()) +" watches.\n";
                }
                else if(*parIt == "parent")
                {
                    simpleUpdate = false;
                    BOOST_FOREACH(Watch* watch, watches)
                    {
                        answer += process_parent(parIt, commandSet.end(), watch);
                    }
                    answer += "Parent setted for "+ IntToStr(watches.size()) +" watches.\n";
                }
                else
                {
                    answer += "Error: unknown argument '"+ *parIt +"' for write command.\n";
                }
            }
            if(simpleUpdate)
            {
                BOOST_FOREACH(Watch* watch, watches)
                {
                    update(watch);
                }
                answer += "Updated " + IntToStr(watches.size()) + " watches.\n";
            }
        }
    }
    else if(firstWord == "write")
    {
        watch = new Watch();
        watch->Type = WriteWatch;   
        watch->Name = _command;
        answer += addWatchCommon(watch, commandSet);
    }
    else if(firstWord == "stop")
    {
        answer += process_stop_resume(commandSet.begin(), commandSet.end(), true);
    }
    else if(firstWord == "resume")
    {
        answer +=  process_stop_resume(commandSet.begin(), commandSet.end(), false);
    }
    else if(firstWord == "close")
    {
        appManager().setRunning(false);
        answer += "Programm terminated.\n";
    }
    else if(firstWord == "help")
    {
        std::string param = "@";
        StrIterator argIt = commandSet.begin() +1;
        if(argIt != commandSet.end())
        {
            param += *argIt;
            boost::trim(param);
        }
        if(boost::filesystem::exists("help"))
        {
            std::ifstream fshelp;
            fshelp.open("help");
            if(fshelp.good())
            {
                bool found = false;
                std::string line;
                while(!found && !fshelp.eof())
                {
                    fshelp >> line;
                    boost::trim(line);
                    if(line == param) found = true;
                    LOG(line);
                }
                if(found)
                {
                    char c = '\n';
                    do
                    {
                        answer += c;
                        fshelp.get(c);
                    }
                    while(c != '$' && !fshelp.eof());  
                }
                else
                {
                    answer += "Error: can't find help for '"+ param +"'.\n";
                }
                fshelp.close();
            }
            else
            {
                answer += "Error: can't read help-file.\n";
            }
        }
        else
        {
            answer += "Error: can't find help-file.\n";
        }
    }
    else if(firstWord == "hello")
    {
        answer += "Hello! Nice to see you!";
    }
    else
    {
        answer += "Error: unknown command '" + commandSet[0] + "'.\n";
    }
}

std::string DebugWatcher::process_parent(StrIterator cmdIt, StrIterator endIt, Watch* watch)
{
    std::string answer = "";
    if(cmdIt+1 != endIt)
    {
        std::vector<Watch*> specifiedParent = getWatches(cmdIt +1, endIt, answer, true);
        if(specifiedParent.size() == 1)
        {
            Watch* parent = specifiedParent[0];
            watch->Parent = parent;
            parent->Children.push_back(watch);
            notifyConsoleChangedParent(watch);
        }
        else if(specifiedParent.size() == 0)
        {
            answer += "Error: not found any watch, specified as parent.\n";
        }
        else
        {
            answer += "Error: too many parents.\n";
        }
    }
    else
    {
        answer += "Error: argument not specified for 'parent' parameter.\n";
    }
    return answer;
}

int DebugWatcher::processEvery(StrIterator everyIt, StrIterator end, std::string& answer)
{
    int result = DEFAULT_TIMEOUT;
    StrIterator argIt = everyIt +1;
    if(argIt != end)
    {
        std::string arg = *argIt;
        if(arg == "step")
        {
            result = EVERY_STEP;
            answer += "Update every step setted.\n";
        }
        else if(arg == "eon" || arg == "aeon")
        {
            result = AEON;
            answer += "Update interval setted to infinity.\n";
        }
        else
        {
            try
            {
                result = boost::lexical_cast<int>(arg);
                answer += "Update timeout set to " + IntToStr(mTimeout) + ".\n";                   
            }
            catch(boost::bad_lexical_cast &)
            {
                answer += "Error: bad argument for 'every' command (excepted 'step' or number of milleseconds).\n";
            }
        }
    }
    else
    {
        answer += "Error: argument not specified for 'every' command.\n";
    }
    return result;
}

EnvironObject* getEnvironObject(std::string _name)
{
    std::string name(_name);
    boost::to_lower(name);
    EnvironObject* result = NULL;
    if(name == "sky")result = environManager().getTypeHandle(Environ_Sky);
    else if(name == "stars")result = environManager().getTypeHandle(Environ_Stars);  
    else if(name == "moon")result = environManager().getTypeHandle(Environ_Moon);  
    else if(name == "lightnings")result = environManager().getTypeHandle(Environ_Lightnings);   
    else if(name == "rain")result = environManager().getTypeHandle(Environ_Rain);  
    else if(name == "clouds")result = environManager().getTypeHandle(Environ_Clouds);  
    else if(name == "leaves")result = environManager().getTypeHandle(Environ_Leaves); 
    else if(name == "birds")result = environManager().getTypeHandle(Environ_Birds); 

    return result;
}

std::string DebugWatcher::addWatchCommon(Watch* watch, std::vector<std::string> &commandSet)
{
    watch->ID = worldManager().generateUniqueID();
    std::string answer = "";
    answer += "Constructing watch with id='"+ watch->ID +"'.\n";
    bool watchNormal = false;
    for(StrIterator it = ++(commandSet.begin()); it != commandSet.end(); ++it)
    {
        std::string command = *it;
        if(command == "as")
        {
            ++it;
            if(it != commandSet.end())
            {
                watch->Name = *it;
                watch->ID = *it;
            }
            else
            {
                answer += "Error: argument not specified for 'as' parameter.\n";
            }
        }
        else if( command == "to")
        {
            if(watch->Type == WriteWatch)
            {
                ++it;
                if(it != commandSet.end())
                {
                    std::string file = *it;
                    boost::trim(file);
                    answer += assignWatchToFile(watch, file, true);
                    BOOST_FOREACH(Watch* child, watch->Children)
                    {
                        answer += assignWatchToFile(child, file, true);
                    }
                }
                else
                {
                    answer += "Error: argument not specified for 'as' parameter.\n";
                }
            }
            else
            {
                answer += "Error: parameter 'to' allowed only for 'write' command.\n";
            }
            watchNormal = true;
        }
        else if( command == "parent")
        {
            answer += process_parent(commandSet.begin() +2, commandSet.end(), watch);
        }
        else if( command == "every" )
        {
            StrIterator argIt = ++it;
            if(argIt != commandSet.end())
            {
                std::string arg = *argIt;
                int interval = DEFAULT_TIMEOUT;
                bool argOk = false;
                if(arg == "step")
                {
                    interval = EVERY_STEP;
                    argOk = true;
                }
                else if(arg == "eon" || arg == "aeon")
                {
                    interval = AEON;
                    argOk = true;
                }
                else
                {
                    try
                    {
                        interval = boost::lexical_cast<int>(arg);
                        argOk = true;
                    }
                    catch(boost::bad_lexical_cast &e)
                    {
                        argOk = false;
                    }
                }
                if(argOk)
                {
                    watch->UpdateInterval = interval;
                    BOOST_FOREACH(Watch* child, watch->Children)
                    {
                        child->UpdateInterval = interval;
                    }
                }
                else
                {
                    answer = "Error: bad argument for every parameter.\n";
                }
            }
            else
            {
                answer += "Error: argument for every not specified.\n";
            }
        }
        //material and others commands can contain brakes at end
        else if( command.find("material") != command.npos)
        {
            std::map<Target, TargetInfo> targets = getTargets(it+1, commandSet.end(), tBodyMaterial, answer);
            answer += add_member_watch(watch, *it, BodyMaterialFields,BodyMaterialFieldsCount, targets, evalute_material);
            watchNormal = true;
        }
        else if( command.find("state") != command.npos)
        {
            std::map<Target, TargetInfo> targets = getTargets(it+1, commandSet.end(), tBodyState, answer);
            answer += add_member_watch(watch, *it, BodyStateFields, BodyStateFieldsCount, targets, evalute_state);
            watchNormal = true;
        }
        else if( command.find("b2param") != command.npos)
        {
            // Solve, what user want b2Fixture or b2Body parameter
            if(it +2 != commandSet.end())
            {
                std::string targetSpecifier =  *(it+2);
                // If exists squares in target definition
                if(targetSpecifier.find("(") != targetSpecifier.npos)
                {
                    // b2Fixture
                    std::map<Target, TargetInfo> targets = getTargets(it+1, commandSet.end(), tb2Fixture, answer);
                    answer += add_member_watch(watch, *it, b2FixtureFields, b2FixtureFieldsCount, targets, evalute_b2Fixture);
                    watchNormal = true;
                }
                else
                {
                    // b2Body
                    std::map<Target, TargetInfo> targets = getTargets(it+1, commandSet.end(), tb2Body, answer);
                    answer += add_member_watch(watch, *it, b2BodyFields,  b2BodyFieldsCount, targets, evalute_b2Body);
                    watchNormal = true;
                }
            }
            else
            {
                answer += "Error: unspecified 'of' parameter or it's argument.\n";
            }
        }
        else if( command.find("param") != command.npos)
        {
            // Solve, what user want b2Fixture or b2Body parameter
            if(it +2 != commandSet.end())
            {
                std::string targetSpecifier =  *(it+2);
                // If exists squares in target definition
                if(targetSpecifier.find("(") != targetSpecifier.npos)
                {
                    //BodyPart
                    std::map<Target, TargetInfo> targets = getTargets(it+1, commandSet.end(), tBodyPart, answer);
                    answer += add_member_watch(watch, *it, BodyPartFields, BodyPartFieldsCount, targets, evalute_BodyPart);
                    watchNormal = true;
                }
                else
                {
                    //Body
                    std::map<Target, TargetInfo> targets = getTargets(it+1, commandSet.end(), tBody, answer);
                    answer += add_member_watch(watch, *it, BodyFields,BodyFieldsCount, targets, evalute_Body);
                    watchNormal = true;
                }
            }
            else
            {
                answer += "Error: unspecified 'of' parameter or it's argument.\n";
            }   
        }
        else if( command.find("environ") != command.npos)
        {
            std::map<Target, TargetInfo> targets = getTargets(it+1, commandSet.end(), tEnvironObject, answer);           
            answer += add_member_watch(watch, *it, EnvironObjectFields, EnvironObjectFieldsCount, targets, evalute_EnvironObject);
            watchNormal = true;
        }
        else if( command.find("elapsed") != command.npos)
        {
            watch->Name = "Elapsed";
            watch->Active = true;
            watch->Object = &appManager();
            watch->Expression = evalute_elapsed;
            watch->MemberName = "Elapsed";
            addWatchToConsole(watch);
            watchNormal = true;
        }
      
    }
    if(watch->Type != NotAWatch && watchNormal)
    {
        mWatches.push_back(watch);
        answer += "Added result watch with id = '" + watch->ID + "'.\n";
    }
    else
    {
        if(!watchNormal)
        {
            answer += "Error: parameters not specified.\n";
        }
        answer += "Watch not added.\n";
    }
    return answer;
}

std::string DebugWatcher::add_member_watch(Watch* watch, std::string command,
         const std::string members[], const int memberCount, std::map<Target, TargetInfo>& targets, EvaluteFunction evalute)
{
    std::string answer = "";
    answer += "Selected " + IntToStr(targets.size()) + " targets.\n";
    if(targets.size() > 0)
    {
        watch->Active = false;
        addWatchToConsole(watch);
        std::map<Target, std::string>::iterator it;
        unsigned int startParamBraket = command.find("(");
        std::vector<std::string> membersToAdd;
        std::string membersToAddStr = "";
        // If members not specified, add to watch all members
        if(startParamBraket == command.npos)
        {
            for(int i=0; i < memberCount; ++i)
            {
                membersToAdd.push_back(std::string(members[i]));
                membersToAddStr += members[i] + ", ";
            }
        }
        // Look specified members only
        else
        {
            std::vector<std::string> specMembers;
            // Command looks like command(Member1, Member2)
            boost::split(specMembers, command, boost::is_any_of("(,)"));
            if( specMembers.size() > 2)
            {
                // First element in split is command
                specMembers.erase(specMembers.begin());
                // Last element is epmty
                specMembers.erase(--specMembers.end());
            }
            BOOST_FOREACH(std::string memberName, specMembers)
            {
                boost::trim(memberName);

                // Check, if member name is normal
                bool normal = false;
                for(int i=0;i < memberCount; ++i)
                {
                    if(members[i] == memberName) 
                    {
                        normal = true;
                        break;
                    }
                }
                if(normal)
                {
                    membersToAdd.push_back(memberName);
                    membersToAddStr += memberName + ", ";
                }
                else
                {
                    answer += "Error: member " + memberName + " not exists.\n";
                }
            }
        }
        //Remove last comma and space
        if( membersToAddStr.size() > 2) 
        {
            membersToAddStr.erase(membersToAddStr.end() - 2, membersToAddStr.end());
            answer += "Members to watch: " + membersToAddStr + ". \n";
        }
        else
        {
            answer += "Error: nothing added to watch.\n";
            watch->Type = NotAWatch;
            return answer;
        }

        std::string targetsStr = "";
        std::map<Body*, Watch*> parentBodyWatches;
        std::map<Target, TargetInfo>::iterator tit;
        for(tit = targets.begin(); tit != targets.end(); ++tit)
        {
            Target target = tit->first;
            std::string partName = tit->second.first; 
            Body* parentBody = tit->second.second;
            bool bodyTarget = target.type() == typeid(parentBody) ||  target.type() == typeid(parentBody->getb2Body());
            Watch* parentWatch = NULL;
            std::map<Body*, Watch*>::iterator parentWatchIt = parentBodyWatches.find(parentBody);
            if(parentWatchIt == parentBodyWatches.end())
            {
                parentWatch = new Watch(*watch);
                parentWatch->Type = watch->Type;
                parentWatch->Name = "Body " + parentBody->getName();
                parentWatch->Active = false;
                parentWatch->Parent = watch;
                watch->Children.push_back(parentWatch);
                parentWatch->ID = watch->ID + "." + parentBody->getName();
                parentWatch->UpdateInterval =  watch->UpdateInterval;
                if(watch->OutFile != NULL)
                {
                    answer += assignWatchToFile(parentWatch, watch->OutFile, true);
                }
                mWatches.push_back(parentWatch);
                addWatchToConsole(parentWatch);
                targetsStr += "Body '"+ parentBody->getName() +"'";
                if(bodyTarget) targetsStr += ", ";
                else targetsStr += ": ";
            }
            else
            {
                parentWatch = parentWatchIt->second;
            }
            if(!bodyTarget)
            {
                targetsStr += "part '"+ partName +"', ";
            }
            BOOST_FOREACH(const std::string memberName, membersToAdd)
            {
                // Watches like Shape.Center are stuff
                if(memberName.find(".") == memberName.npos)
                {
                    Watch* child = new Watch(*parentWatch);
                    child->Name = memberName;
                    child->Type = parentWatch->Type;
                    child->Active = true;
                    child->Object = target;
                    child->Expression = evalute;
                    child->MemberName = memberName;
                    child->Parent = parentWatch;
                    parentWatch->Children.push_back(child);
                    child->ID = parentWatch->ID + "." + partName + "(" + memberName + ")";
                    child->UpdateInterval =  parentWatch->UpdateInterval;
                    if(parentWatch->OutFile != NULL)
                    {
                        answer += assignWatchToFile(child, parentWatch->OutFile, true);
                    }
                    mWatches.push_back(child);
                    addWatchToConsole(child);
                }
            }
        }
         //Remove last comma and space
        targetsStr.erase(targetsStr.end() - 2, targetsStr.end());
        answer += "Targets: " + targetsStr + ". \n"; 
    }//if(targets.size() > 0)
    else
    {
        watch->Type = NotAWatch;
    }

    return answer;
}


std::map<Target, TargetInfo> DebugWatcher::getTargets(StrIterator command, StrIterator end, TargetType type, std::string& answer)
{
    std::map<Target, TargetInfo> result;
    if(type & tApplicationManager)
    {
        result.insert(std::pair<Target, TargetInfo>(&appManager(), TargetInfo("ApplicationManager", NULL)));
    }
    if(type & tEnvironObject)
    {
        if( *command == "of")
        {
            StrIterator argument = command +1;
            if(argument != end)
            {
                EnvironObject* target = getEnvironObject(*argument);
                if(target != NULL)
                {
                    result.insert(std::pair<Target, TargetInfo>(target, TargetInfo(*argument, NULL)));
                }
                else
                {
                    answer += "Error: invalid argument '" + *argument + "'.\n";
                }
            }
            else
            {
                 answer += "Error: parameter 'of' not specified.\n";
            }
        } 
        else
        {
            answer += "Error: parameter 'of' not specified.\n";
        } 
    }//if(type & tEnvironObject)
    // Commands synapsis: of objName(fixture_number_or_name)
    if(command != end)
    {
        if( *command == "of")
        {
            StrIterator argument = command +1;
            if(argument != end)
            {
                std::string targetSpecifier = *argument;
                bool all = targetSpecifier == "all";
                std::vector<std::string> objNameAndFixtures;
                boost::split(objNameAndFixtures, targetSpecifier, boost::is_any_of("(,)"));
                std::string objName = objNameAndFixtures[0];
                std::list<Body*> bodies = physicManager().getBodies();
                std::list<Body*>::iterator it;
                for(it = bodies.begin(); it != bodies.end(); ++it)
                {
                    if((*it)->getName() == objName || all)
                    {
                        Body* obj = *it;
                        if(type & tBody)
                        {
                           result.insert(std::pair<Target, TargetInfo>(obj, TargetInfo(objName, obj)));
                        }
                        if(type & tb2Body)
                        {
                            result.insert(std::pair<Target, TargetInfo>(obj->getb2Body(), TargetInfo(objName, obj)));
                        }
                        // If parts specified
                        if(objNameAndFixtures.size() > 1)
                        {
                            std::vector<std::string>::iterator it2;
                            for(it2 = ++(objNameAndFixtures.begin()); 
                                  it2 != objNameAndFixtures.end(); ++it2)
                            {
                                std::string partID = *it2;
                                if(partID != "")
                                {
                                    std::vector<b2Fixture*> fixtures = getFixtures(obj, &partID);
                                    BOOST_FOREACH(b2Fixture* fixture, fixtures)
                                    {
                                        if(type & tb2Fixture)
                                        {
                                            result.insert(std::pair<Target, TargetInfo>(fixture, TargetInfo(partID, obj)));
                                        }
                                        BodyPart* part = (BodyPart*)fixture->GetUserData();
                                        if(part != NULL)
                                        {
                                            if(type & tBodyPart)
                                            {
                                                result.insert(std::pair<Target, TargetInfo>(part, TargetInfo(partID, obj)));
                                            }
                                            if(type & tBodyMaterial)
                                            {
                                                result.insert(std::pair<Target, TargetInfo>(part->getMaterial(), TargetInfo(partID, obj)));
                                            }
                                            if(type & tBodyState)
                                            {
                                                result.insert(std::pair<Target, TargetInfo>(part->getState(), TargetInfo(partID, obj)));
                                            }                                    
                                        }
                                    }
                                    if(fixtures.size() == 0)
                                    {
                                        answer += "Error: no part indetified as '" + partID + "'.\n";
                                        //continue
                                    }
                                }//if(partID != "")
                            }
                        }//if(objNameAndFixtures.size() > 1) 
                        else 
                        {
                            // Get all parts
                            b2Body* body = obj->getb2Body();
                            int i = 0;
                            for(b2Fixture* fixture = body->GetFixtureList(); 
                                     fixture != NULL; fixture = fixture->GetNext())
                            {
                                std::string id = IntToStr(i);

                                BodyPart* part = (BodyPart*)fixture->GetUserData();
                                if(part != NULL)
                                {
                                    id = part->getName();
                                    if(type & tBodyPart)
                                    {
                                        result.insert(std::pair<Target, TargetInfo>(part, TargetInfo(id, obj)));
                                    }
                                    if(type & tBodyMaterial)
                                    {
                                        result.insert(std::pair<Target, TargetInfo>(part->getMaterial(),  TargetInfo(id, obj)));
                                    }
                                    if(type & tBodyState)
                                    {
                                        result.insert(std::pair<Target, TargetInfo>(part->getState(),  TargetInfo(id, obj)));
                                    }
                                }
                                if(type & tb2Fixture)
                                {
                                    result.insert(std::pair<Target, TargetInfo>(fixture,  TargetInfo(id, obj)));
                                }
                                ++i;
                            }
                        }
                    }//if((*it)->mName == objName)
                }//for(it = bodies.begin(); it != bodies.end(); ++it)
            }//if(argument != end)
            else
            {
                answer += "Error: argument for parameter 'of' not specified!\n";
            }
        }//if( *command == "of")
        else
        {
             answer += "Error: parameter 'of' not specified.\n";
        }
    } // if(command != end)
    else
    {
        answer += "Error: parameter 'of' not specified.\n";
    }
    return result;
}


std::vector<b2Fixture*> DebugWatcher::getFixtures(Body* obj, std::string* partID)
{
    std::vector<b2Fixture*> result;
    b2Body* body = obj->getb2Body();
    if(*partID == "all")
    {
         for(b2Fixture* fixture = body->GetFixtureList(); 
                     fixture != NULL; fixture = fixture->GetNext())
         {
            result.push_back(fixture);
         }
    }
    else
    {
        bool numberID = false;
        int partNumber = 0;
        try
        {
            partNumber = boost::lexical_cast<int>(*partID);
            numberID = true;
        }
        catch(boost::bad_lexical_cast &e)
        {
            numberID = false;
        }
        if(numberID)
        {
            int currentPartNumber = 0;
            for(b2Fixture* fixture = body->GetFixtureList(); 
                     fixture != NULL; fixture = fixture->GetNext())
            {
                if(currentPartNumber == partNumber)
                {
                    result.push_back(fixture);
                    BodyPart* part = (BodyPart*)fixture->GetUserData();
                    if(part!= NULL)
                    {
                        partID->assign(part->getName());
                    }
                    break;
                }
                currentPartNumber++;
            }
        }
        else
        {
            for(b2Fixture* fixture = body->GetFixtureList(); 
                     fixture != NULL; fixture = fixture->GetNext())
            {
                BodyPart* part = (BodyPart*)fixture->GetUserData();
                if(part != NULL)
                {
                    if(part->getName() == *partID)
                    {
                        result.push_back(fixture);
                        partID->assign(part->getName());
                        break;
                    }
                }
            }
        }
    }
    
    
    return result;
}

std::vector<Watch*> DebugWatcher::getWatches(StrIterator specIt, StrIterator endIt, std::string& answer, bool children = true)
{
    std::vector<Watch*> watches;
    if(specIt != endIt)
    {
        std::string param = *specIt;
        if(param == "last")
        {
            if(mWatches.size() > 0)
            {
                Watch* watch = mWatches.back();
                watches.push_back(watch);
                if(children)
                {
                    BOOST_FOREACH(Watch* child, watch->Children)
                    {
                        watches.push_back(child);
                    }
                }
            }
            else 
            {
                answer += "Error: can't find last watch: no one exists.\n";
            }
        }
        else if(param == "all")
        {
            for(std::list<Watch*>::iterator it = mWatches.begin(); it != mWatches.end(); ++it)
                watches.push_back(*it);
        }
        else if(param == "selected")
        {
             answer += "Error: not supported yet.\n";
        }
        else
        {
           Watch* watch = getWatchByID(param);
           if(watch != NULL)
            {
                watches.push_back(watch);
                if(children)
                {
                    BOOST_FOREACH(Watch* child, watch->Children)
                    {
                        watches.push_back(child);
                    }
                }
            }
            else
            {
                 answer += "Error: watch with id '" + param + "' not exist.\n";
            }
        }
    }
    else
    {
         answer += "Error: watch not specified.\n";
    }
    return watches;
}

std::string DebugWatcher::process_remove(StrIterator commandIt, StrIterator endIt)
{
    std::string answer = "";
    int removed = 0;
    StrIterator parIt = commandIt +1;
    if(parIt != endIt)
    {
        if(*parIt == "watch")
        {
            std::vector<Watch*> watches = getWatches(parIt + 1, endIt, answer);
            BOOST_FOREACH(Watch* watch, watches)
            {
                if(watch->OutFile != NULL)
                {
                    answer += unassignWatchFromFile(watch, false);
                }
                removeWatchFromConsole(watch);
                removed++;
                mWatches.remove(watch);
                if(watch->Parent != NULL)
                {
                    watch->Parent->Children.remove(watch);
                    watch->Parent = NULL;
                }
            }
            BOOST_FOREACH(Watch* watch, watches)
            {
                delete watch;
            }
            answer += "Removed " + IntToStr(removed) + " watches.\n"; 
        }
        else if(*parIt == "object")
        {
            answer += "Coming soon...\n";
        }
        else
        {
            answer += "Error: unknown parameter '" + *parIt + "' for command remove.\n";
        }
    }
    else
    {
        answer += "Error: parameter not specified for remove comamand.\n";
    }
    return answer;
}

std::string DebugWatcher::process_hide(StrIterator commandIt, StrIterator endIt)
{
    std::string answer = "";
    int hiden = 0;
    StrIterator parIt = commandIt +1;
    bool childrenHide = false;
    // hide children of <watch>
    if(parIt != endIt && parIt+1 != endIt && parIt+2 != endIt)
    {
        if(*parIt == "children" && *(parIt+1) == "of")
        {
            childrenHide = true;
            std::vector<Watch*> watches = getWatches(parIt+2, endIt, answer, false);
            BOOST_FOREACH(Watch* watch, watches)
            {
                BOOST_FOREACH(Watch* child, watch->Children)
                {
                    if(child->OutFile != NULL)
                    {
                        answer += unassignWatchFromFile(child, false);
                    }
                    child->Active = false;
                    removeWatchFromConsole(child);
                    hiden++; 
                }                
            }
        }
    }
    if(!childrenHide)
    {
        std::vector<Watch*> watches = getWatches(commandIt + 1, endIt, answer);
        BOOST_FOREACH(Watch* watch, watches)
        {
            if(watch->OutFile != NULL)
            {
                answer += unassignWatchFromFile(watch, false);
            }
            watch->Active = false;
            removeWatchFromConsole(watch);
            hiden++;            
        }
    }
    answer += "Hiden " + IntToStr(hiden) + " watches.\n"; 
    return answer;
}



std::string DebugWatcher::process_stop_resume(StrIterator commandIt, StrIterator endIt, bool stop)
{
    std::string answer = "";
    int processed = 0;
    StrIterator argIt = commandIt + 1;
    if(argIt != endIt)
    {
        if(*argIt == "file")
        {
            StrIterator filePathIt = argIt + 1;
            if(filePathIt != endIt)
            {
                std::string path = *filePathIt;
                if(path == "all")
                {
                    BOOST_FOREACH(Watch* watch, mWatches)
                    {
                        if(watch->OutFile != NULL)
                        {
                            watch->Active = !stop;
                            if(stop)
                            {
                              answer += unassignWatchFromFile(watch, false);
                            }
                            else
                            {
                                answer += assignWatchToFile(watch, watch->OutFile, false);
                            }
                            processed++;
                        }
                    }
                }
                else
                {
                    boost::trim(path);
                    std::map<std::string, std::ofstream*>::iterator fit = mFiles.find(path);
                    if(fit != mFiles.end())
                    {
                        std::ofstream* fs = fit->second;
                        BOOST_FOREACH(Watch* watch, mWatches)
                        {
                            if(watch->OutFile == fs)
                            {
                                watch->Active = !stop;
                                if(stop)
                                  answer += unassignWatchFromFile(watch, false);
                                else
                                    answer += assignWatchToFile(watch, watch->OutFile, false);

                                processed++;
                            }
                        }
                    }
                    else
                    {
                        answer += "Error: file '"+ path +"' is not using now.\n";
                    }
                }
            }//if(filePathIt != endIt)
            else
            {
                answer += "Error: file path not specified for 'stop file' or 'resume file' command.\n";
            }
        }//if(*argIt == "file")
        else
        {
            std::vector<Watch*> watches = getWatches(argIt, endIt, answer);
            BOOST_FOREACH(Watch* watch, watches)
            {
                if(watch->Active == stop)
                {
                    watch->Active = !stop;
                    processed++;
                }
                if(watch->OutFile != NULL)
                {
                    if(stop)
                    {
                        answer += unassignWatchFromFile(watch, false);
                    }
                    else
                    {   
                        answer += assignWatchToFile(watch, watch->OutFile, false);
                    }
                }
            }
        }
    }//if(argIt != endIt)
    else
    {
        answer += "Error: argument not specified for command 'stop' or 'resume'.\n";
    }
    std::string processedStr = (stop)? "Stopped ":"Resumed ";
    answer += processedStr + IntToStr(processed) + " watches.\n"; 
    return answer;
}

Watch* DebugWatcher::getWatchByID(std::string id)
{
    Watch* watch = NULL;
    for (std::list<Watch*>::iterator it = mWatches.begin();
            it != mWatches.end(); ++it)
    {
        if((*it)->ID == id)
        {
            watch = *it;
            break;
        }
    }
    return watch;
}

std::string DebugWatcher::assignWatchToFile(Watch* watch, std::string file, bool rewrite)
{
    std::string answer = "";
    std::ofstream *fs = NULL;
    bool reopen = false;
    if(watch->OutFile != NULL)
    {
        // If assignation to different file
        if(getFileName(watch->OutFile) != file)
        {
            answer += unassignWatchFromFile(watch, false);
        }
        else
        {
            fs = watch->OutFile;
            reopen = true;
        }
    }
    std::map<std::string, std::ofstream*>::iterator fit = mFiles.find(file);
    if(fit == mFiles.end() || reopen)
    {
        if (boost::filesystem::exists(file) && rewrite)
        {
            answer += "Rewriting file '"+ file +"'\n";
        }
        if(!reopen)
        {
            fs = new std::ofstream();
        }
        if(rewrite)
        {
            fs->open(file.c_str(), std::ios_base::trunc);
        }
        else
        {
            fs->open(file.c_str(), std::ios_base::app);
        }
        if(fs->is_open())
        {
            if(!reopen)
            {
                fit = mFiles.insert(std::pair<std::string, std::ofstream*>(file, fs)).first;
            }
            answer += "Opened file '" + file + "' for write.\n";
        }
        else
        {
            answer += "Error: can't open file '" + file + "'.\n";
            watch->Type = NotAWatch;
        }
    }
    if(fit != mFiles.end())
    {
        watch->OutFile = fit->second;
        answer += "Watch '" + watch->ID + "' assigned to file '" + fit->first + "'.\n";
        std::map<std::ofstream*, int>::iterator fileUseIt = mFilesUsing.find(fit->second);
        if(fileUseIt == mFilesUsing.end())
        {
            mFilesUsing.insert(std::pair<std::ofstream*, int>(fit->second, 1));
        }
        else
        {
            fileUseIt->second++;
        }
    }
    return answer;
}

std::string DebugWatcher::assignWatchToFile(Watch* watch, std::ofstream* file, bool rewrite)
{
    std::string answer = "";
    answer += assignWatchToFile( watch, getFileName(file), rewrite);
    return answer;
}

std::string DebugWatcher::unassignWatchFromFile(Watch* watch, bool dispose)
{
    std::string answer = "";
    std::map<std::ofstream*, int>::iterator fileUseIt = mFilesUsing.find(watch->OutFile);
    if(dispose)
    {
        watch->OutFile = NULL;
    }
    if(fileUseIt != mFilesUsing.end())
    {
        fileUseIt->second--;
        std::string filePath = "";
        if(fileUseIt->second == 0)
        {
            std::ofstream* fs = fileUseIt->first;

            std::map<std::string, std::ofstream*>::iterator fit;
            for(fit = mFiles.begin(); fit != mFiles.end(); )
            {
                if(fit->second == fs)
                {
                    break;
                }
                ++fit;
            }
            if(fit != mFiles.end())
            {
                filePath = fit->first;
                if(dispose) 
                {
                    mFiles.erase(fit);
                }
            }
            if(dispose) 
            { 
                mFilesUsing.erase(fileUseIt);
            }

            fs->flush();
            fs->close();
            if(dispose)
            {
                delete fs;
            }
            answer += "Closed file '" + filePath + "'.\n";
        }
        else
        {
            if(fileUseIt->first->is_open())
            {
                fileUseIt->first->flush();
            }
        }
        answer += "Watch with id = '"+ watch->ID +"' detached from file '"+ filePath+"'.\n";
    }
    return answer;
}

std::string DebugWatcher::getFileName(std::ofstream* file)
{
    std::string result = "";
    std::map<std::string, std::ofstream*>::iterator fit;
    for(fit = mFiles.begin(); fit != mFiles.end(); )
    {
        if(fit->second == file)
        {
            break;
        }
        ++fit;
    }
    if(fit != mFiles.end())
    {
        result = fit->first;
    }
    return result;
}

void DebugWatcher::step()
{
    int elapsed = appManager().getElapsed();
    BOOST_FOREACH(Watch* watch, mWatches)
    {
        if(watch->UpdateInterval == AEON)
        {
            continue;
        }
        else if(watch->UpdateInterval == EVERY_STEP)
        {
            update(watch);
        }
        else
        {
            watch->LeftFromLastUpdate += elapsed;
            if(watch->LeftFromLastUpdate >= watch->UpdateInterval)
            {
                watch->LeftFromLastUpdate = 0;
                update(watch);
            }
        }
    }
}

void DebugWatcher::update(Watch* watch)
{
    if( watch->Active)
    {

        if( watch->Type == ShowWatch)
        {

            updateWatchInConsole(watch);
        }
        else if(watch->Type == WriteWatch)
        {
            if(watch->OutFile == NULL)
            {
                Watch* next = new Watch(*watch);
                next->ID = watch->ID + "." + worldManager().generateUniqueID();
                next->Parent = watch;
                watch->Children.push_back(next);
                addWatchToConsole(next);
            }
            else
            {
                if(watch->OutFile->is_open())
                {
                    std::string record = "";
                    std::string time = boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time());
                    record = time + " : [" + watch->ID + "] " + watch->Name + " = " + watch->Expression(watch)+ "\n";
                    watch->OutFile->write(record.c_str(), record.size());                        
                }
                else
                {
                    LOG("Error: file was closed for '" + watch->ID + "' watch. Deacitvating watch...");
                    watch->Active = false;
                }
            }
        }
    }
}


void DebugWatcher::addWatchToConsole(Watch* watch)
{
    std::string parentID = "";
    if(watch->Parent != NULL)
    {
        parentID = watch->Parent->ID;
    }
    debugIO().addWatch(watch->ID, watch->Name, watch->Expression(watch), parentID);
}
void DebugWatcher::updateWatchInConsole(Watch* watch)
{
    debugIO().updateWatch(watch->ID, watch->Expression(watch));
}
void DebugWatcher::removeWatchFromConsole(Watch* watch)
{
    debugIO().removeWatch(watch->ID);
}

void DebugWatcher::notifyConsoleChangedParent(Watch* watch)
{
    LOG("Changing parent for " + watch->ID);
}
