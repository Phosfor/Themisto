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
    else if(commandSet[0] == "stop write")
    {
        answer = process_stop(commandSet);
    }
    else if( commandSet[0] == "close")
    {
    }
    else if(commandSet[0] == "every")
    {
        if(commandSet.size() > 1)
        {
            if(commandSet[1] == "step")
            {
                mTimeout = -1;
            }
            else
            {
                try
                {
                    mTimeout = boost::lexical_cast<int>(commandSet[1]);                   
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
    else if(commandSet[0] == "quit")
    {
         appManager.setRunning(false);
    }
    else if(commandSet[0] == "say hello")
    {
        LOG("\nHello\n");
    }
    else
    {
        answer += "Error: unknown command '" + commandSet[0] + "'\n.";
    }
}

string DebugWatcher::addWatchCommon(Watch* watch, vector<string> &commandSet)
{
    string error = "";
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
                error += "Error: argument not specified for 'as' parameter.\n";
            }
        }
        else if( command == "to")
        {
        }
        //material and others commands can contain brakes at end
        else if( command.find("material") != command.npos)
        {
            error += process_material(watch, commandSet);
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
    mWatches.push_back(watch);
    return error;
}
string intToStr(int p)
{
    std::ostringstream out;
    out << p;
    return out.str(); 
}


map<Target, string>& DebugWatcher::getTargets(vector<string> &commandSet, TargetType type, string& error)
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
    if(commandSet.size() > 3)
    {
        if( commandSet[2] == "of")
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
                                error += "Error: no part indetified as '" + partID + "'.\n";
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
                            string id = intToStr(i);
                           
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
       
        }//if( commandSet[2] == "of")
        else
        {
             error += "Error: parameter 'of' not specified.\n";
        }
    } //if(commandSet.size() > 3)
    else
    {
        error += "Error: parameter 'of' not specified.\n";
    }
    return *result;
}

string floatToStr(float p)
{
    std::ostringstream out;
    out << p;
    return out.str(); 
}




string evalute_material(Watch* watch)
{
    string name = watch->MemberName;
    string result = "error, member not found";
    BodyMaterial* m = boost::get<BodyMaterial*>(watch->Object);
    if(name == "Name") result = m->Name;
    else if(name == "KindleTemperature") result = floatToStr(m->KindleTemperature);
    else if(name == "KindleReceptivity") result = floatToStr(m->KindleReceptivity);
    else if(name == "FlameTemperature") result = floatToStr(m->FlameTemperature);
    else if(name == "SelfFlareUpRate") result = floatToStr(m->SelfFlareUpRate);
    else if(name == "CarbonizeRate") result = floatToStr(m->CarbonizeRate);
    else if(name == "ElectricalConductivity") result = floatToStr(m->ElectricalConductivity);
    else if(name == "ThermalReceptivity") result = floatToStr(m->ThermalReceptivity);
    else if(name == "DampReceptivity") result = floatToStr(m->DampReceptivity);
    else if(name == "FrozingTemperature") result = floatToStr(m->FrozingTemperature);
    else if(name == "InflDampnessToFriction") result = floatToStr(m->InflDampnessToFriction);
    else if(name == "InflDampnessToKindleTemperature") result = floatToStr(m->InflDampnessToKindleTemperature);
    else if(name == "InflDampnessToMaxKindle") result = floatToStr(m->InflDampnessToMaxKindle);
    else if(name == "InflDampnessToKindleReceptivity") result = floatToStr(m->InflDampnessToKindleReceptivity);
    else if(name == "InflDampnessToFrozingTemperature") result = floatToStr(m->InflDampnessToFrozingTemperature);
    else if(name == "InflCarbonizeLevelToMaxKindle") result = floatToStr(m->InflCarbonizeLevelToMaxKindle);
    else if(name == "InflCarbonizeLevelToMaxDampness") result = floatToStr(m->InflCarbonizeLevelToMaxDampness);
    else if(name == "InflCarbonizeLevelToElecticalConductivity") result = floatToStr(m->InflCarbonizeLevelToElecticalConductivity);
    else if(name == "InflMoistenToKindleLevel") result = floatToStr(m->InflMoistenToKindleLevel);
    else if(name == "InflTemperatureToDampness") result = floatToStr(m->InflTemperatureToDampness);  
    
    return result;
}

string DebugWatcher::process_material(Watch* watch, vector<string> &commandSet)
{
    addWatchToConsole(watch);
    string error = "";
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
    
    map<Target, string> targets = getTargets(commandSet, tBodyMaterial, error);
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
    }
    return error;
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
    string error = "";
    if(commandSet.size() > 1)
    {
        string param = commandSet[1];
        if(param == "last")
        {
            if(mWatches.size() > 0)
            {
                Watch* watch = mWatches.back();
                removeWatchFromConsole(watch);
                mWatches.remove(watch);
                delete watch;
            }
            else 
            {
                error += "Error: can't hide last watch: no one exists.\n";
            }
        }
        else if(param == "all")
        {
            for(list<Watch*>::iterator it = mWatches.begin(); it != mWatches.end(); ++it)
            {
                Watch* watch = mWatches.back();
                removeWatchFromConsole(watch);
                delete watch;
            }
            mWatches.clear();
        }
        else if(param == "selected")
        {
             error += "Error: not supported yet.\n";
        }
        else
        {
           Watch* watch = getWatchByID(param);
           if(watch != NULL)
            {
                mWatches.remove(watch);
                removeWatchFromConsole(watch);
                delete watch;
            }
            else
            {
                 error += "Error: watch with id '" + param + "' not exist.\n";
            }
        }
    }
    else
    {
        error += "Error: argument not specified for 'hide' command.\n";
    }
    return error;
}

string DebugWatcher::process_stop(vector<string>& commandSet)
{
    string error = "";
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
                    found = true;
                    break;
                }   
            }
            if(!found)
            {
                error += "Error: can't stop writing last watch: no one exists.\n";
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
                }   
            }
            
        }
        else if(param == "selected")
        {
            error += "Error: not supported yet.\n";
        }
        else
        {
           Watch* watch = getWatchByID(param);
           if(watch != NULL)
            {
                if(watch->Type == WriteWatch)
                {
                    watch->Active = false;
                }   
                else
                {
                    error += "Error: watch with id '" + param + "' is not writing (it is showing, use hide).\n";
                }
            }
            else
            {
                error += "Error: watch with id '" + param + "' not found.\n";
            }
        }
    }
    else
    {
        error += "Error: argument not specified for 'stop writing' command.\n";
    }
    return error;
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
    Type = ShowWatch;
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
    //addWatch(watch->ID, watch->Name, watch->Expression(), parentID);
}
void DebugWatcher::updateWatchInConsole(Watch* watch)
{
    //updateWatch(watch->ID, watch->Expression());
}
void DebugWatcher::removeWatchFromConsole(Watch* watch)
{
    //removeWatch(watch->ID);
}
















