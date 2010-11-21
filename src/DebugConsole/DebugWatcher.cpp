#include "DebugConsole/DebugWatcher.hpp"

void DebugWatcher::init()
{
    //subsribe to commands
    mCommandSlot = debugIO.mCommand.connect(this, &DebugWatcher::parseCommand);
    
    mTimeout = DEFAULT_TIMEOUT;   
    mLeftFromLastUpdate = 0;
    _appManager = &appManager;
}

void DebugWatcher::parseCommand(string command, string* _answer)
{

    if(command.size() == 0) return;
    string& answer = *_answer;
    // Normalize command
    boost::trim(command);
    while(command.find("  ") != command.npos)
    {
        boost::replace_all(command, "  ", " ");
    }
    
    std::vector<std::string> commandSet;
    boost::split(commandSet, command, boost::is_any_of(" "));
    Watch* watch;
    if(commandSet[0] == "show")
    {
        watch = new Watch();
        watch->Type = ShowWatch;
        answer += addWatchCommon(watch, commandSet);
    }
    else if(commandSet[0] == "hide")
    {
        answer += process_hide(commandSet);
    }
    else if(commandSet[0] == "update")
    {
        update();
    }
    else if(commandSet[0] == "write")
    {
        watch = new Watch();
        watch->Type = WriteWatch;	
        answer += addWatchCommon(watch, commandSet);
    }
    else if(commandSet[0] == "stop")
    {
        answer = process_stop(commandSet);
    }
    else if( commandSet[0] == "close")
    {
        appManager.setRunning(false);
        answer += "Programm terminated.\n";
    }
    else if(commandSet[0] == "every")
    {
        if(commandSet.size() > 1)
        {
            if(commandSet[1] == "step")
            {
                mTimeout = -1;
                answer += "Update every step setted. \n";
            }
            else
            {
                try
                {
                    mTimeout = boost::lexical_cast<int>(commandSet[1]);
                    answer += "Update timeout set to " + IntToStr(mTimeout) + 
                    "; Note, that update intervals not certain equal to timeout. \n";                   
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
    }
    else if( commandSet[0] == "execute")
    {
        answer += "Coming soon... :)\n.";
    }
    else if(commandSet[0] == "help")
    {
        answer += "Coming soon... :)\n.";
    }
    else if(commandSet[0] == "hello")
    {
        answer += "Hello! How are you?";
    }
    else
    {
        answer += "Error: unknown command '" + commandSet[0] + "'.\n";
    }
}

string DebugWatcher::addWatchCommon(Watch* watch, vector<string> &commandSet)
{
    watch->ID = worldManager.generateUniqueID();
    string answer = "";
    bool watchNormal = false;
    for(vector<string>::iterator it = ++(commandSet.begin()); it != commandSet.end(); ++it)
    {
        string command = *it;
        if(command == "as")
        {
            ++it;
            if(it != commandSet.end())
            {
                watch->Name = *it;
            }
            else
            {
                answer += "Error: argument not specified for 'as' parameter.\n";
            }
        }
        else if( command == "to")
        {
        }
        //material and others commands can contain brakes at end
        else if( command.find("material") != command.npos)
        {
            answer += process_material(watch, commandSet);
            watchNormal = true;
        }
        else if( command.find("state") != command.npos)
        {
        }
        else if( command.find("b2param") != command.npos)
        {
        }
        else if( command.find("param") != command.npos)
        {
        }
        else if( command.find("environ") != command.npos)
        {
        }
        else if( command.find("elapsed") != command.npos)
        {
        }     
    }
    if(watch->Type != NotAWatch && watchNormal)
    {
        mWatches.push_back(watch);
        answer += "Added result watch with id = '" + watch->ID + "'.\n";
    }
    return answer;
}



map<Target, string>& DebugWatcher::getTargets(vector<string> &commandSet, TargetType type, string& answer)
{
    map<Target, string>* result = new map<Target, string>();
    if(type & tApplicationManager)
    {
        result->insert(pair<Target, string>(&appManager, "ApplicationManager"));
    }
    if(type & tEnvironObject)
    {
        result->insert(pair<Target, string>( &environManager, "EnvironManager"));
    }
    // Commands synapsis: pam pam of objName(fixture_number_or_name)
    if(commandSet.size() > 2)
    {
        if( commandSet[2] == "of")
        {
            if(commandSet.size() > 3)
            {
                string targetSpecifier = commandSet[3];
                std::vector<std::string> objNameAndFixtures;
                boost::split(objNameAndFixtures, targetSpecifier, boost::is_any_of("(,)"));
                string objName = objNameAndFixtures[0];
                list<Body*> bodies = physicManager.getBodies();
                list<Body*>::iterator it;
                for(it = bodies.begin(); it != bodies.end(); ++it)
                {
                    if((*it)->mName == objName)
                    {
                        Body* obj = *it;
                        if(type & tBody)
                        {
                           result->insert(pair<Target, string>(obj, objName));
                        }
                        if(type & tb2Body)
                        {
                            result->insert(pair<Target, string>(obj->getb2Body(), objName));
                        }
                        // If parts specified
                        if(objNameAndFixtures.size() > 1)
                        {
                            vector<string>::iterator it;
                            for(it = ++(objNameAndFixtures.begin()); 
                                  it != objNameAndFixtures.end(); ++it)
                            {
                                string partID = *it;
                                b2Fixture* fixture = getFixture(obj, partID);
                                if(fixture != NULL)
                                {
                                    if(type & tb2Fixture)
                                    {
                                        result->insert(pair<Target, string>(fixture, partID));
                                    }
                                    BodyPart* part = (BodyPart*)fixture->GetUserData();
                                    if(part != NULL)
                                    {
                                        if(type & tBodyPart)
                                        {
                                            result->insert(pair<Target, string>(part, partID));
                                        }
                                        if(type & tBodyMaterial)
                                        {
                                            result->insert(pair<Target, string>(part->getMaterial(), partID));
                                        }
                                        if(type & tBodyState)
                                        {
                                            result->insert(pair<Target, string>(part->getState(), partID));
                                        }                                    
                                    }
                                }
                                else
                                {
                                    answer += "Error: no part indetified as '" + partID + "'.\n";
                                    //continue
                                }
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
                                    id = part->mName;
                                    if(type & tBodyPart)
                                    {
                                        result->insert(pair<Target, string>(part, id));
                                    }
                                    if(type & tBodyMaterial)
                                    {
                                        result->insert(pair<Target, string>(part->getMaterial(), id));
                                    }
                                    if(type & tBodyState)
                                    {
                                        result->insert(pair<Target, string>(part->getState(), id));
                                    }                                    
                                }     
                                if(type & tb2Fixture)
                                {
                                    result->insert(pair<Target, string>(fixture, id));
                                } 
                                ++i;     
                            }
                        }
                    }//if((*it)->mName == "objName")
                }//for(it = bodies.begin(); it != bodies.end(); ++it)
            }//if( commandSet.size() > 3)
            else
            {
                answer += "Error: argument for parameter 'of' not specified.\n";
            }
       
        }//if( commandSet[2] == "of")
        else
        {
             answer += "Error: parameter 'of' not specified.\n";
        }
    } //if(commandSet.size() > 3)
    else
    {
        answer += "Error: parameter 'of' not specified.\n";
    }
    return *result;
}

string evalute_material(Watch* watch)
{
    string name = watch->MemberName;
    string result = "Error, member not found";
    BodyMaterial* m = boost::get<BodyMaterial*>(watch->Object);
    if(name == "Name") result = m->Name;
    else if(name == "KindleTemperature") result = FloatToStr(m->KindleTemperature);
    else if(name == "KindleReceptivity") result = FloatToStr(m->KindleReceptivity);
    else if(name == "FlameTemperature") result = FloatToStr(m->FlameTemperature);
    else if(name == "SelfFlareUpRate") result = FloatToStr(m->SelfFlareUpRate);
    else if(name == "CarbonizeRate") result = FloatToStr(m->CarbonizeRate);
    else if(name == "ElectricalConductivity") result = FloatToStr(m->ElectricalConductivity);
    else if(name == "ThermalReceptivity") result = FloatToStr(m->ThermalReceptivity);
    else if(name == "DampReceptivity") result = FloatToStr(m->DampReceptivity);
    else if(name == "FrozingTemperature") result = FloatToStr(m->FrozingTemperature);
    else if(name == "InflDampnessToFriction") result = FloatToStr(m->InflDampnessToFriction);
    else if(name == "InflDampnessToKindleTemperature") result = FloatToStr(m->InflDampnessToKindleTemperature);
    else if(name == "InflDampnessToMaxKindle") result = FloatToStr(m->InflDampnessToMaxKindle);
    else if(name == "InflDampnessToKindleReceptivity") result = FloatToStr(m->InflDampnessToKindleReceptivity);
    else if(name == "InflDampnessToFrozingTemperature") result = FloatToStr(m->InflDampnessToFrozingTemperature);
    else if(name == "InflCarbonizeLevelToMaxKindle") result = FloatToStr(m->InflCarbonizeLevelToMaxKindle);
    else if(name == "InflCarbonizeLevelToMaxDampness") result = FloatToStr(m->InflCarbonizeLevelToMaxDampness);
    else if(name == "InflCarbonizeLevelToElecticalConductivity") result = FloatToStr(m->InflCarbonizeLevelToElecticalConductivity);
    else if(name == "InflMoistenToKindleLevel") result = FloatToStr(m->InflMoistenToKindleLevel);
    else if(name == "InflTemperatureToDampness") result = FloatToStr(m->InflTemperatureToDampness);  
    
    return result;
}

string DebugWatcher::process_material(Watch* watch, vector<string> &commandSet)
{
    
    string answer = "";
    string materialCommand = commandSet[1];
    unsigned int startParamBraket = materialCommand.find("(");
    string fields[20] = {
        "Name",
        "KindleTemperature",
        "KindleReceptivity",
        "FlameTemperature",
        "SelfFlareUpRate",
        "CarbonizeRate",
        "ElectricalConductivity",
        "ThermalReceptivity",
        "DampReceptivity",
        "FrozingTemperature",
        "InflDampnessToFriction",
        "InflDampnessToKindleTemperature",
        "InflDampnessToMaxKindle",
        "InflDampnessToKindleReceptivity",
        "InflDampnessToFrozingTemperature",
        "InflCarbonizeLevelToMaxKindle",
        "InflCarbonizeLevelToMaxDampness",
        "InflCarbonizeLevelToElecticalConductivity",
        "InflMoistenToKindleLevel",
        "InflTemperatureToDampness"
    };
    
    map<Target, string> targets = getTargets(commandSet, tBodyMaterial, answer);
    answer += "Selected " + IntToStr(targets.size()) + " body parts.\n";
    if(targets.size() > 0)
    {
        watch->Active = false;
        addWatchToConsole(watch);
        map<Target, string>::iterator it;
        if(startParamBraket == materialCommand.npos)
        {
            for(it = targets.begin(); it != targets.end(); ++it)
            {
                Target material = it->first;
                string watchName = it->second; 
                for(int i = 0; i<20; i++)
                {
                    Watch* child = new Watch(*watch);
                    child->Name = watchName + "(" + fields[i] + ")";
                    child->Object = material;
                    child->Expression = evalute_material;
                    child->MemberName = fields[i];
                    child->Parent = watch;
                    watch->Children.push_back(child);
                    child->ID = watch->ID + "." + watchName + "(" + fields[i] + ")";
                    mWatches.push_back(child);
                    addWatchToConsole(child);
                } 
            }
            answer += "All material properties added to watch.\n";
        }
        else
        {
            std::vector<std::string> fields;
            boost::split(fields, materialCommand, boost::is_any_of("(,)"));
            for(it = targets.begin(); it != targets.end(); ++it)
            {
                Target material = it->first;
                string watchName = it->second; 
                vector<string>::iterator it;
                for(it = ++(fields.begin()); it != fields.end(); ++it)
                { 
                    Watch* child = new Watch(*watch);
                    string field = *it;
                    boost::trim(field);
                    child->Name = watchName + "(" + field + ")";;
                    child->Object = material;
                    child->Expression = evalute_material;
                    child->MemberName = field;
                    child->Parent = watch;
                    watch->Children.push_back(child);
                    child->ID = watch->ID + "." + watchName + "(" + field + ")";
                    mWatches.push_back(child);
                    addWatchToConsole(child);
                }
            }
            answer += "To watch added next properties of material: ";
            vector<string>::iterator it;
            for(it = ++(fields.begin()); it != fields.end(); ++it)
            {
                answer += *it;
            } 
            answer += ". \n";
        }
    }//if(targets.size() > 0)
    else
    {
        watch->Type = NotAWatch;
    }
    return answer;
}

b2Fixture* DebugWatcher::getFixture(Body* obj, string partID)
{
    b2Fixture* result = NULL;
    b2Body* body = obj->getb2Body();
    bool numberID = false;
    
    int partNumber = 0;
    try
    {
        partNumber = boost::lexical_cast<int>(partID);
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
                if(part->mName == partID)
                {
                    result = fixture;
                }
            }
        }
    }
    return result;
}





string DebugWatcher::process_hide(vector<string>& commandSet)
{
    string answer = "";
    int removed = 0;
    if(commandSet.size() > 1)
    {
        string param = commandSet[1];
        if(param == "last")
        {
            if(mWatches.size() > 0)
            {
                Watch* watch = mWatches.back();
                removeWatchFromConsole(watch);
                removed++;
                mWatches.remove(watch);
                delete watch;
            }
            else 
            {
                answer += "Error: can't hide last watch: no one exists.\n";
            }
        }
        else if(param == "all")
        {
            for(list<Watch*>::iterator it = mWatches.begin(); it != mWatches.end(); ++it)
            {
                Watch* watch = mWatches.back();
                removeWatchFromConsole(watch);
                removed++;
                delete watch;
            }
            mWatches.clear();
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
                mWatches.remove(watch);
                removeWatchFromConsole(watch);
                removed++;
                delete watch;
            }
            else
            {
                 answer += "Error: watch with id '" + param + "' not exist.\n";
            }
        }
    }
    else
    {
        answer += "Error: argument not specified for 'hide' command.\n";
    }
    answer += "Hiden " + IntToStr(removed) + " watches.\n";
    return answer;
}

string DebugWatcher::process_stop(vector<string>& commandSet)
{
    string answer = "";
    int stopped = 0;
    if(commandSet.size() > 1)
    {
        string param = commandSet[1];
        if(param == "last")
        {
            bool found = false;
            for(list<Watch*>::reverse_iterator rit = mWatches.rbegin(); 
                    rit != mWatches.rend(); ++rit)
            {
                Watch* watch = *rit;
                if(watch->Type == WriteWatch)
                {
                    watch->Active = false;
                    stopped++;
                    found = true;
                    break;
                }   
            }
            if(!found)
            {
                answer += "Error: can't stop writing last watch: no one exists.\n";
            }
        }
        else if(param == "all")
        {
            for(list<Watch*>::iterator it = mWatches.begin(); it != mWatches.end(); ++it)
            {
                Watch* watch = mWatches.back();
                if(watch->Type == WriteWatch)
                {
                    watch->Active = false;
                    stopped++;
                }   
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
                if(watch->Type == WriteWatch)
                {
                    watch->Active = false;
                    stopped++;
                }   
                else
                {
                    answer += "Error: watch with id '" + param + "' is not writing (it is showing, use hide).\n";
                }
            }
            else
            {
                answer += "Error: watch with id '" + param + "' not found.\n";
            }
        }
    }
    else
    {
        answer += "Error: argument not specified for 'stop writing' command.\n";
    }
    answer += "Stopped " + IntToStr(stopped) + " watches.\n";
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

void DebugWatcher::step()
{
    //every step enabled
    if(mTimeout<0)
    {
        update();
    }
    else
    {
        if(mLeftFromLastUpdate >= mTimeout)
        {
            mLeftFromLastUpdate = 0;
            update();
        }
        else
        {
            mLeftFromLastUpdate += _appManager->getElapsed();
        }
    }
}

void DebugWatcher::update()
{
    list<Watch*>::iterator it;
    for(it = mWatches.begin(); it != mWatches.end(); ++it)
    {
        Watch* watch = *it;
        if( watch->Active)
        {
            if( watch->Type == ShowWatch)
            {
                updateWatchInConsole(watch);
            }
            else if(watch->Type == WriteWatch)
            {
                Watch* next = new Watch(*watch);
                next->ID = worldManager.generateUniqueID();
                addWatchToConsole(next);
            }
        }
    }
}

string nope(Watch* watch)
{
    return "";
}

Watch::Watch()
{
    Active = true;
    Object = (void*)NULL;
    MemberName = "";
    OutFile = "";
    Parent = NULL;
    Type = NotAWatch;
    ID = "";
    Name = "";
    Expression = nope;
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
















