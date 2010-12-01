#include "Debug/DebugWatcher.hpp"

void DebugWatcher::init()
{
    //subsribe to commands
    mCommandSlot = debugIO.mCommand.connect(this, &DebugWatcher::parseCommand);
    
    mTimeout = DEFAULT_TIMEOUT;   
    mLeftFromLastUpdate = 0;
    _appManager = &appManager;
}

DebugWatcher::~DebugWatcher()
{
    string answer = "";
    parseCommand("hide all", &answer);
    LOG("~DebugWatcher: " + answer);
}



void DebugWatcher::parseCommand(string _command, string* _answer)
{
    
    if(_command.size() == 0) return;
    string& answer = *_answer;
    // Normalize command
    string command(_command);
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
    string firstWord = commandSet[0];
    Watch* watch;
    if(firstWord == "show")
    {
        watch = new Watch();
        watch->Type = ShowWatch;
        watch->Name = _command;
        answer += addWatchCommon(watch, commandSet);
    }
    else if(firstWord == "hide")
    {
        answer += process_hide(commandSet.begin(), commandSet.end());
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
            vector<Watch*> watches = getWatches(argIt, commandSet.end(), answer);
            StrIterator everyIt = argIt +1;
            bool simpleUpdate = true;
            if(everyIt != commandSet.end())
            {
                if(*everyIt == "every")
                {
                    simpleUpdate = false;
                    int every = processEvery(argIt +1, commandSet.end(), answer);
                    BOOST_FOREACH(Watch* watch, watches)
                    {
                        watch->UpdateInterval = every;
                    }
                    answer += "Update interval setted for "+ IntToStr(watches.size()) +" watches.\n";
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
        appManager.setRunning(false);
        answer += "Programm terminated.\n";
    }
    else if(firstWord == "help")
    {
        string param = "@";
        StrIterator argIt = commandSet.begin() +1;
        if(argIt != commandSet.end())
        {
            param += *argIt;
            boost::trim(param);
        }
        if(boost::filesystem::exists("help"))
        {
            ifstream fshelp;
            fshelp.open("help");
            if(fshelp.good())
            {
                bool found = false;
                string line;
                while(!found && !fshelp.eof())
                {
                    fshelp >> line;
                    boost::trim(line);
                    if(line == param)
                    {
                        found = true;
                    }
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

int DebugWatcher::processEvery(StrIterator everyIt, StrIterator end, string& answer)
{
    int result = DEFAULT_TIMEOUT;
    StrIterator argIt = everyIt +1;
    if(argIt != end)
    {
        string arg = *argIt;
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

EnvironObject* getEnvironObject(string _name)
{
    string name(_name);
    boost::to_lower(name);
    EnvironObject* result = NULL;
    if(name == "sky")result = environManager.getTypeHandle(Environ_Sky);
    else if(name == "stars")result = environManager.getTypeHandle(Environ_Stars);  
    else if(name == "moon")result = environManager.getTypeHandle(Environ_Moon);  
    else if(name == "lightnings")result = environManager.getTypeHandle(Environ_Lightnings);   
    else if(name == "rain")result = environManager.getTypeHandle(Environ_Rain);  
    else if(name == "clouds")result = environManager.getTypeHandle(Environ_Clouds);  
    else if(name == "leaves")result = environManager.getTypeHandle(Environ_Leaves); 
    else if(name == "birds")result = environManager.getTypeHandle(Environ_Birds); 
    
    return result;
}

string DebugWatcher::addWatchCommon(Watch* watch, vector<string> &commandSet)
{
    watch->ID = worldManager.generateUniqueID();
    string answer = "";
    answer += "Constructing watch with id='"+ watch->ID +"'.\n";
    bool watchNormal = false;
    for(StrIterator it = ++(commandSet.begin()); it != commandSet.end(); ++it)
    {
        string command = *it;
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
                    string file = *it;
                    boost::trim(file);
                    answer += assignWatchToFile(watch, file);
                    BOOST_FOREACH(Watch* child, watch->Children)
                    {
                        answer += assignWatchToFile(child, file);
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
        else if( command == "every" )
        {
            StrIterator argIt = ++it;
            if(argIt != commandSet.end())
            {
                string arg = *argIt;
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
            map<Target, string> targets = getTargets(it+1, commandSet.end(), tBodyMaterial, answer);
            answer += add_member_watch(watch, *it, BodyMaterialFields,BodyMaterialFieldsCount, targets, evalute_material);
            watchNormal = true;
        }
        else if( command.find("state") != command.npos)
        {
            map<Target, string> targets = getTargets(it+1, commandSet.end(), tBodyState, answer);
            answer += add_member_watch(watch, *it, BodyStateFields, BodyStateFieldsCount, targets, evalute_state);
            watchNormal = true;
        }
        else if( command.find("b2param") != command.npos)
        {
            // Solve, what user want b2Fixture or b2Body parameter
            if(it +2 != commandSet.end())
            {
                string targetSpecifier =  *(it+2);
                // If exists squares in target definition
                if(targetSpecifier.find("(") != targetSpecifier.npos)
                {
                    // b2Fixture
                    map<Target, string> targets = getTargets(it+1, commandSet.end(), tb2Fixture, answer);
                    answer += add_member_watch(watch, *it, b2FixtureFields, b2FixtureFieldsCount, targets, evalute_b2Fixture);
                    watchNormal = true;
                }
                else
                {
                    // b2Body
                    map<Target, string> targets = getTargets(it+1, commandSet.end(), tb2Body, answer);
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
                string targetSpecifier =  *(it+2);
                // If exists squares in target definition
                if(targetSpecifier.find("(") != targetSpecifier.npos)
                {
                    //BodyPart
                    map<Target, string> targets = getTargets(it+1, commandSet.end(), tBodyPart, answer);
                    answer += add_member_watch(watch, *it, BodyPartFields, BodyPartFieldsCount, targets, evalute_BodyPart);
                    watchNormal = true;
                }
                else
                {
                    //Body
                    map<Target, string> targets = getTargets(it+1, commandSet.end(), tBody, answer);
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
            map<Target, string> targets = getTargets(it+1, commandSet.end(), tEnvironObject, answer);           
            answer += add_member_watch(watch, *it, EnvironObjectFields, EnvironObjectFieldsCount, targets, evalute_EnvironObject);
            watchNormal = true;
        }
        else if( command.find("elapsed") != command.npos)
        {
            watch->Name = "Elapsed";
            watch->Active = true;
            watch->Object = &appManager;
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

string DebugWatcher::add_member_watch(Watch* watch, string command,
         const string members[], const int memberCount, map<Target, string>& targets, EvaluteFunction evalute)
{
    string answer = "";
    answer += "Selected " + IntToStr(targets.size()) + " targets.\n";
    if(targets.size() > 0)
    {
        watch->Active = false;
        addWatchToConsole(watch);
        map<Target, string>::iterator it;
        unsigned int startParamBraket = command.find("(");
        string added = "";
        // If members not specified, add to watch all members
        if(startParamBraket == command.npos)
        {
            for(it = targets.begin(); it != targets.end(); ++it)
            {
                Target target = it->first;
                string watchName = it->second; 
                
                for(int i=0; i < memberCount; ++i)
                {
                    const string memberName = members[i];
                    // Watches like Shape.Center are stuff
                    if(memberName.find(".") == memberName.npos)
                    {
                        Watch* child = new Watch(*watch);
                        child->Name = memberName;
                        child->Type = watch->Type;
                        child->Active = true;
                        child->Object = target;
                        child->Expression = evalute;
                        child->MemberName = memberName;
                        child->Parent = watch;
                        watch->Children.push_back(child);
                        child->ID = watch->ID + "." + watchName + "(" + memberName + ")";
                        child->UpdateInterval =  watch->UpdateInterval;
                        answer += assignWatchToFile(child, watch->OutFile);
                        mWatches.push_back(child);
                        addWatchToConsole(child);
                        added += memberName + ", ";
                    }
                } 
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
            
            for(it = targets.begin(); it != targets.end(); ++it)
            {
                Target target = it->first;
                string watchName = it->second; 
                
                BOOST_FOREACH(string memberName, specMembers)
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
                        Watch* child = new Watch(*watch);
                        child->Name = memberName;
                        child->Type = watch->Type;
                        child->Active = true;
                        child->Object = target;
                        child->Expression = evalute;
                        child->MemberName = memberName;
                        child->Parent = watch;
                        watch->Children.push_back(child);
                        child->ID = watch->ID + "." + watchName + "(" + memberName + ")";
                        child->UpdateInterval =  watch->UpdateInterval;
                        answer += assignWatchToFile(child, watch->OutFile);
                        mWatches.push_back(child);
                        addWatchToConsole(child);
                        added += memberName + ", ";
                    }
                    else
                    {
                        answer += "Error: member " + memberName + " not exists.\n";
                    }
                }// BOOST_FOREACH(string memberName, specMembers)
            }//for(it = targets.begin(); it != targets.end(); ++it)
        }
        //Remove last comma and space
        if( added.size() > 2) 
        {
            added.erase(added.end() - 2, added.end());
            answer += "To watch added next properties: " + added + ". \n";
        }
        else
        {
            answer += "Error: nothing added to watch.\n";
            watch->Type = NotAWatch;
        }
        
    }//if(targets.size() > 0)
    else
    {
        watch->Type = NotAWatch;
    }


    return answer;
}


map<Target, string> DebugWatcher::getTargets(StrIterator command, StrIterator end, TargetType type, string& answer)
{
    map<Target, string> result;
    if(type & tApplicationManager)
    {
        result.insert(pair<Target, string>(&appManager, "ApplicationManager"));
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
                    result.insert(pair<Target, string>(target, *argument));
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
    // Commands synapsis: pam pam of objName(fixture_number_or_name)
    if(command != end)
    {
        if( *command == "of")
        {
            StrIterator argument = command +1;
            if(argument != end)
            {
                string targetSpecifier = *argument;
                std::vector<std::string> objNameAndFixtures;
                boost::split(objNameAndFixtures, targetSpecifier, boost::is_any_of("(,)"));
                string objName = objNameAndFixtures[0];
                list<Body*> bodies = physicManager.getBodies();
                list<Body*>::iterator it;
                for(it = bodies.begin(); it != bodies.end(); ++it)
                {
                    if((*it)->getName() == objName)
                    {
                        Body* obj = *it;
                        if(type & tBody)
                        {
                           result.insert(pair<Target, string>(obj, objName));
                        }
                        if(type & tb2Body)
                        {
                            result.insert(pair<Target, string>(obj->getb2Body(), objName));
                        }
                        // If parts specified
                        if(objNameAndFixtures.size() > 1)
                        {
                            vector<string>::iterator it;
                            for(it = ++(objNameAndFixtures.begin()); 
                                  it != objNameAndFixtures.end(); ++it)
                            {
                                string partID = *it;
                                if(partID != "")
                                {
                                    b2Fixture* fixture = getFixture(obj, &partID);
                                    if(fixture != NULL)
                                    {
                                        if(type & tb2Fixture)
                                        {
                                            result.insert(pair<Target, string>(fixture, partID));
                                        }
                                        BodyPart* part = (BodyPart*)fixture->GetUserData();
                                        if(part != NULL)
                                        {
                                            if(type & tBodyPart)
                                            {
                                                result.insert(pair<Target, string>(part, partID));
                                            }
                                            if(type & tBodyMaterial)
                                            {
                                                result.insert(pair<Target, string>(part->getMaterial(), partID));
                                            }
                                            if(type & tBodyState)
                                            {
                                                result.insert(pair<Target, string>(part->getState(), partID));
                                            }                                    
                                        }
                                    }
                                    else
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
                                string id = IntToStr(i);
                               
                                BodyPart* part = (BodyPart*)fixture->GetUserData();
                                if(part != NULL)
                                {
                                    id = part->getName();
                                    if(type & tBodyPart)
                                    {
                                        result.insert(pair<Target, string>(part, id));
                                    }
                                    if(type & tBodyMaterial)
                                    {
                                        result.insert(pair<Target, string>(part->getMaterial(), id));
                                    }
                                    if(type & tBodyState)
                                    {
                                        result.insert(pair<Target, string>(part->getState(), id));
                                    }                                    
                                }     
                                if(type & tb2Fixture)
                                {
                                    result.insert(pair<Target, string>(fixture, id));
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


b2Fixture* DebugWatcher::getFixture(Body* obj, string* partID)
{
    b2Fixture* result = NULL;
    b2Body* body = obj->getb2Body();
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
                result = fixture;
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
                    result = fixture;
                    partID->assign(part->getName());
                    break;
                }
            }
        }
    }
    return result;
}

vector<Watch*> DebugWatcher::getWatches(StrIterator specIt, StrIterator endIt, string& answer)
{
    vector<Watch*> watches;
    if(specIt != endIt)
    {
        string param = *specIt;
        if(param == "last")
        {
            if(mWatches.size() > 0)
            {
                Watch* watch = mWatches.back();
                BOOST_FOREACH(Watch* child, watch->Children)
                {
                    watches.push_back(child);
                }
                watches.push_back(watch);
            }
            else 
            {
                answer += "Error: can't find last watch: no one exists.\n";
            }
        }
        else if(param == "all")
        {
            for(list<Watch*>::iterator it = mWatches.begin(); it != mWatches.end(); ++it)
            {
                watches.push_back(*it);
            }
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
                BOOST_FOREACH(Watch* child, watch->Children)
                {
                    watches.push_back(child);
                }
                watches.push_back(watch);
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

string DebugWatcher::process_hide(StrIterator commandIt, StrIterator endIt)
{
    string answer = "";
    int removed = 0;
    vector<Watch*> watches = getWatches(commandIt + 1, endIt, answer);
    BOOST_FOREACH(Watch* watch, watches)
    {
        if(watch->OutFile != NULL)
        {
            answer += unassignWatchFromFile(watch);
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
    answer += "Hiden " + IntToStr(removed) + " watches.\n"; 
    return answer;
}



string DebugWatcher::process_stop_resume(StrIterator commandIt, StrIterator endIt, bool stop)
{
    string answer = "";
    int processed = 0;
    StrIterator argIt = commandIt + 1;
    if(argIt != endIt)
    {
        if(*argIt == "file")
        {
            StrIterator filePathIt = argIt + 1;
            if(filePathIt != endIt)
            {
                string path = *filePathIt;
                if(path == "all")
                {
                    BOOST_FOREACH(Watch* watch, mWatches)
                    {
                        if(watch->OutFile != NULL)
                        {
                            watch->Active = !stop;
                            if(stop)
                            {
                              answer += unassignWatchFromFile(watch);
                            }
                            processed++;
                        }
                    }
                }
                else
                {
                    boost::trim(path);
                    map<string, ofstream*>::iterator fit = mFiles.find(path);
                    if(fit != mFiles.end())
                    {
                        ofstream* fs = fit->second;
                        BOOST_FOREACH(Watch* watch, mWatches)
                        {
                            if(watch->OutFile == fs)
                            {
                                watch->Active = !stop;
                                if(stop)
                                {
                                  answer += unassignWatchFromFile(watch);
                                }
                                processed++;
                            }
                        }
                    }
                    else
                    {
                        answer += "Error: file '"+ path +"' is not using now.\n";
                    }
                }
            }
            else
            {
                answer += "Error: file path not specified for 'stop file' or 'resume file' command.\n";
            }
        }
        else
        {
            vector<Watch*> watches = getWatches(argIt, endIt, answer);
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
                        answer += unassignWatchFromFile(watch);
                    }
                }
            }
        }
    }
    else
    {
        answer += "Error: argument not specified for command 'stop' or 'resume'.\n";
    }
    string processedStr = (stop)? "Stopped ":"Resumed ";
    answer += processedStr + IntToStr(processed) + " watches.\n"; 
    return answer;
}

Watch* DebugWatcher::getWatchByID(string id)
{
    Watch* watch = NULL;
    for(list<Watch*>::iterator it = mWatches.begin();
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

string DebugWatcher::assignWatchToFile(Watch* watch, string file)
{
    string answer = "";
    
    if(watch->OutFile != NULL)
    {
        answer += unassignWatchFromFile(watch);
    }
    map<string, ofstream*>::iterator fit = mFiles.find(file);
    if(fit == mFiles.end())
    {
        if (boost::filesystem::exists(file))
        {
            answer += "Rewriting file '"+ file +"'\n";
        }
        ofstream *fs = new ofstream();
        fs->open(file.c_str(), ios_base::trunc);
        if(fs->is_open())
        {
            fit = mFiles.insert(pair<string, ofstream*>(file, fs)).first;
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
        map<ofstream*, int>::iterator fileUseIt = mFilesUsing.find(fit->second);
        if(fileUseIt == mFilesUsing.end())
        {
            mFilesUsing.insert(pair<ofstream*, int>(fit->second, 1));
        }
        else
        {
            fileUseIt->second++;
        }
    }
    return answer;
}

string DebugWatcher::assignWatchToFile(Watch* watch, ofstream* file)
{
    string answer = "";
    map<string, ofstream*>::iterator fit;
    for(fit = mFiles.begin(); fit != mFiles.end(); ++fit)
    {
        if(fit->second == file)
        {
            break;
        }
    }
    if(fit != mFiles.end())
    {
        string filePath = fit->first;
        answer += assignWatchToFile( watch, filePath);
    }
    return answer;
}

string DebugWatcher::unassignWatchFromFile(Watch* watch)
{
    string answer = "";
    map<ofstream*, int>::iterator fileUseIt = mFilesUsing.find(watch->OutFile);
    watch->OutFile = NULL;
    if(fileUseIt != mFilesUsing.end())
    {
        fileUseIt->second--;
        string filePath = "";
        if(fileUseIt->second == 0)
        {
            ofstream* fs = fileUseIt->first;       
            map<string, ofstream*>::iterator fit;
            for(fit = mFiles.begin(); fit != mFiles.end(); ++fit)
            {
                if(fit->second == fs)
                {
                    break;
                }
            }
            mFilesUsing.erase(fileUseIt);
           
            if(fit != mFiles.end())
            {
                filePath = fit->first;
                mFiles.erase(fit);
            }
            
            fs->flush();
            fs->close();
            delete fs;
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

void DebugWatcher::step()
{
    int elapsed = _appManager->getElapsed();
    BOOST_FOREACH(Watch* watch, mWatches)
    {
        if(watch->UpdateInterval == EVERY_STEP)
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
                next->ID = watch->ID + "." + worldManager.generateUniqueID();
                next->Parent = watch;
                watch->Children.push_back(next);
                addWatchToConsole(next);
            }
            else
            {
                if(watch->OutFile->is_open())
                {
                    string record = "";
                    string time = boost::posix_time::to_simple_string(microsec_clock::local_time());
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
    string parentID = "";
    if(watch->Parent != NULL)
    {
        parentID = watch->Parent->ID;
    }
    debugIO.addWatch(watch->ID, watch->Name, watch->Expression(watch), parentID);
}
void DebugWatcher::updateWatchInConsole(Watch* watch)
{
    debugIO.updateWatch(watch->ID, watch->Expression(watch));
}
void DebugWatcher::removeWatchFromConsole(Watch* watch)
{
    debugIO.removeWatch(watch->ID);
}
















