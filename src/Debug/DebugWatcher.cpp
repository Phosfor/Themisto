#include "Debug/DebugWatcher.hpp"

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

string evalute_state(Watch* watch)
{
    string name = watch->MemberName;
    string result = "Error, member not found";
    BodyState* m = boost::get<BodyState*>(watch->Object); 
    if(name == "IsFrozen") result = FloatToStr(m->IsFrozen);
    else if(name == "KindleLevel") result = FloatToStr(m->KindleLevel);
    else if(name == "CarbonizeLevel") result = FloatToStr(m->CarbonizeLevel);
    else if(name == "Temperature") result = FloatToStr(m->Temperature);
    else if(name == "Dampness") result = FloatToStr(m->Dampness);
    return result;
}

string evalute_b2Fixture(Watch* watch)
{
    string name = watch->MemberName;
    string result = "";
    b2Fixture* m = boost::get<b2Fixture*>(watch->Object); 
                        
             
    string shapeTypePrefix = "", shapeVetexCountPrefix = "",
        shapeVerticesPrefix = "", shapeRadiusPrefix = "", shapeCenterPrefix = "";
    bool shape = false;
    b2Shape* xshape = m->GetShape();
    if(name == "Shape")
    {
        result += "{ ";
        shape = true;
        shapeTypePrefix = "Type = ";
        shapeVetexCountPrefix = ", VertexCount = ";
        shapeVerticesPrefix = ", Vertices = ";
        shapeRadiusPrefix = ", Radius = ";
        shapeCenterPrefix = ", Center = ";
    }
    if(name == "Shape.Type" || shape)
    {
        string type = "";
        if(xshape->GetType() == b2Shape::e_unknown) type = "e_unknown";
        else if(xshape->GetType() == b2Shape::e_circle) type = "e_circle";
        else if(xshape->GetType() == b2Shape::e_polygon) type = "e_polygon";
        else if(xshape->GetType() == b2Shape::e_typeCount) type = "e_typeCount";
        result += shapeTypePrefix + type;
    }
    if(name == "Shape.Center" || shape)
    {
        if(xshape->GetType() == b2Shape::e_circle) 
        {
            b2CircleShape* cshape= (b2CircleShape*)xshape;
            result += shapeCenterPrefix + VectorToStr(cshape->m_p);
        }
        else if(xshape->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape* pshape= (b2PolygonShape*)xshape;
            result += shapeCenterPrefix + VectorToStr(pshape->m_centroid);
        }
    }
    if(name == "Shape.Radius" || shape) result += shapeRadiusPrefix + FloatToStr(m->GetShape()->m_radius);
    if(name == "Shape.VertexCount" || shape)
    {
        if(xshape->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape* pshape= (b2PolygonShape*)xshape;
            result += shapeVetexCountPrefix + IntToStr(pshape->m_vertexCount);
        }
        else 
        {
            if(!shape)
            {
                result = "shape is circle";
            }
        }
    }
    if(name == "Shape.Vertices" || shape)
    {
        if(xshape->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape* pshape= (b2PolygonShape*)xshape;
            result += shapeVerticesPrefix;
            for(int i =0; i<pshape->m_vertexCount; ++i)
            {
                result += VectorToStr(pshape->m_vertices[i]);
                result += " ";
            }
        }
        else 
        {
            if(!shape)
            {
                result = "shape is circle";
            }
        }
    }
    if(shape) result += " }";
    else if(name == "IsSensor") result = FloatToStr(m->IsSensor());
    bool filter = false;
    string filterCategoryPrefix = "", filterMaskPrefix = "", filterGroupPrefix = "";
    if(name == "Filter")
    {
        filter = true;
        result += "{ ";
        filterCategoryPrefix = "CategoryBits = ";
        filterMaskPrefix = ", MaskBits = ";
        filterGroupPrefix = ", GroupIndex = ";
    }               
    if( name == "Filter.CategoryBits" || filter) 
        result += filterCategoryPrefix + IntToStr(m->GetFilterData().categoryBits);
    if( name == "Filter.MaskBits" || filter) 
        result += filterMaskPrefix + HexToStr(m->GetFilterData().maskBits);
    if( name == "Filter.GroupIndex") 
        result += filterGroupPrefix + IntToStr(m->GetFilterData().groupIndex);
    if(filter)
    {
        result += " }";  
    }
    else if(name == "ParentBody")
    {
        Body* body = (Body*)m->GetBody()->GetUserData();
        if(body != NULL)
            result = body->mName;
        else
            result = "Has no atached Body object";
    }
    bool mass = false;
    string massValuePrefix = "", massCenterPrefix = "", massRotationInertiaPrefix = "";
    b2MassData massData;
    m->GetMassData(&massData);
    if( name == "Mass")
    {
        mass = true;
        result += "{ ";
        massValuePrefix = "Value = ";
        massCenterPrefix = ", Center = ";
        massRotationInertiaPrefix = ", RotationInertia = ";
    }
    if( name == "Mass.Value" || mass) result += massValuePrefix + FloatToStr(massData.mass);
    if( name == "Mass.Center" || mass) result += massCenterPrefix + VectorToStr(massData.center);
    if( name == "Mass.RotationInertia" || mass) 
        result += massRotationInertiaPrefix + FloatToStr(massData.I);
    if(mass) result += " }";   

    else if(name == "Density") result = FloatToStr(m->GetDensity());
    else if(name == "Restitution") result = FloatToStr(m->GetRestitution());
    else if(name == "Friction") result = FloatToStr(m->GetFriction());
    
    bool aabb = false;
    string aabbTopPrefix = "", aabbBottomPrefix = "", aabbLeftPrefix = "", aabbRightPrefix = "";
    if( name == "AABB" )
    {
        aabb = true;
        result += "{ ";
        aabbTopPrefix = "Top = ";
        aabbBottomPrefix = "Bottom = ";
        aabbLeftPrefix = "Left = ";
        aabbRightPrefix = "Right = ";
    }
    if( name == "AABB.Top" || aabb) result += aabbTopPrefix + FloatToStr(m->GetAABB().upperBound.y);
    if( name == "AABB.Bottom" || aabb) result += aabbBottomPrefix + FloatToStr(m->GetAABB().lowerBound.y);
    if( name == "AABB.Left" || aabb) result += aabbLeftPrefix + FloatToStr(m->GetAABB().upperBound.x);
    if( name == "AABB.Right" || aabb) result += aabbRightPrefix + FloatToStr(m->GetAABB().upperBound.x);
    if(aabb) result += " }";
    
    if( result == "") result = "Erorr, can't evalute";
    return result;
}
string evalute_b2Body(Watch* watch)
{
    string name = watch->MemberName;
    string result = "";
    b2Body* body = boost::get<b2Body*>(watch->Object); 
    
    if( name == "Type")
    {
        b2BodyType type = body->GetType();
        switch(type)
        {
            case b2_staticBody: result = "StaticBody"; break;
            case b2_dynamicBody: result = "DynamicBody"; break;
            case b2_kinematicBody: result = "KinematicBody"; break;
            default: result = "Unknown type";
        }
    }
    else if(name == "Position") result = VectorToStr(body->GetPosition());
    else if(name == "Angle") result = FloatToStr(body->GetAngle());
    else if(name == "WorldCenter") result = VectorToStr(body->GetWorldCenter());
    else if(name == "LocalCenter") result = VectorToStr(body->GetLocalCenter());
    else if(name == "LinearVelocity") result = VectorToStr(body->GetLinearVelocity());
    else if(name == "AngularVelocity") result = FloatToStr(body->GetAngularVelocity());
    else if(name == "Inertia") result = FloatToStr(body->GetInertia());
    else if(name == "LinearDamping") result = FloatToStr(body->GetLinearDamping());
    else if(name == "AngularDamping")result = FloatToStr(body->GetAngularDamping());
    else if(name == "Bullet") result = BoolToStr(body->IsBullet());
    else if(name == "SleepingAllowed") result = BoolToStr(body->IsSleepingAllowed());
    else if(name == "Awake") result = BoolToStr(body->IsAwake());
    else if(name == "Active") result = BoolToStr(body->IsActive());
    else if(name == "FixedRotation") result = BoolToStr(body->IsFixedRotation());
    bool mass = false;
    string massValuePrefix = "", massCenterPrefix = "", massRotationInertiaPrefix = "";
    b2MassData massData;
    body->GetMassData(&massData);
    if( name == "Mass")
    {
        mass = true;
        result = "{ ";
        massValuePrefix = "Value = ";
        massCenterPrefix = ", Center = ";
        massRotationInertiaPrefix = ", RotationInertia = ";
    }
    if( name == "Mass.Value" || mass) result += massValuePrefix + FloatToStr(massData.mass);
    if( name == "Mass.Center" || mass) result += massCenterPrefix + VectorToStr(massData.center);
    if( name == "Mass.RotationInertia" || mass) 
        result += massRotationInertiaPrefix + FloatToStr(massData.I);
    if(mass) result += " }";   
    
    if(result == "") result = "Can't evalute";
    
    return result;    
}

string evalute_BodyPart(Watch* watch)
{
    string name = watch->MemberName;
    string result = "";
    BodyPart* part = boost::get<BodyPart*>(watch->Object); 
    
    if( name == "IsDefaultMaterial") result = BoolToStr(part->mIsDefaultMaterial);
    else if( name == "MaxKindleLevel") result = FloatToStr(part->mMaxKindleLevel);
    else if( name == "MaxDampness") result = FloatToStr(part->mMaxDampness);
    else if( name == "AcceptsCord") result = BoolToStr(part->mAcceptsCord);
    else if( name == "Name") result = part->mName;
    
    if(result == "") result = "Can't evalute";
    return result;
}

string DebugWatcher::addWatchCommon(Watch* watch, vector<string> &commandSet)
{
    watch->ID = worldManager.generateUniqueID();
    string answer = "";
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
        }
        //material and others commands can contain brakes at end
        else if( command.find("material") != command.npos)
        {
            map<Target, string> targets = getTargets(it+1, commandSet.end(), tBodyMaterial, answer);
            const int count = 20;
            string fields[count] = {
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
            vector<string> members;
            BOOST_FOREACH(string member, fields)
            {
                members.push_back(member);
            }
            answer += add_member_watch(watch, *it, members, targets, evalute_material);
            watchNormal = true;
        }
        else if( command.find("state") != command.npos)
        {
            map<Target, string> targets = getTargets(it+1, commandSet.end(), tBodyState, answer);
            const int count = 5;
            string fields[count] = {
                "IsFrozen",
                "KindleLevel",
                "CarbonizeLevel",
                "Temperature",
                "Dampness"        
            };
            vector<string> members;
            BOOST_FOREACH(string member, fields)
            {
                members.push_back(member);
            }
            answer += add_member_watch(watch, *it, members, targets, evalute_state);
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
                    const int count = 23;
                    string fields[count] = {
                        "Shape",
                        "Shape.Type",
                        "Shape.VertexCount",
                        "Shape.Vertices",
                        "Shape.Radius",
                        "Shape.Center",
                        "IsSensor",
                        "Filter",
                        "Filter.CategoryBits",
                        "Filter.MaskBits",
                        "Filter.GroupIndex",
                        "ParentBody",
                        "Mass",
                        "Mass.Center",
                        "Mass.RotationInertia",
                        "Density",
                        "Restitution",
                        "Friction",
                        "AABB",
                        "AABB.Top",
                        "AABB.Bottom",
                        "AABB.Left",
                        "AABB.Right"
                    };
                    vector<string> members;
                    BOOST_FOREACH(string member, fields)
                    {
                        members.push_back(member);
                    }
                    answer += add_member_watch(watch, *it, members, targets, evalute_b2Fixture);
                    watchNormal = true;
                }
                else
                {
                    // b2Body
                    map<Target, string> targets = getTargets(it+1, commandSet.end(), tb2Body, answer);
                    const int count = 18;
                    string fields[count] = {
                        "Type",
                        "Position",
                        "Angle",
                        "WorldCenter",
                        "LocalCenter",
                        "LinearVelocity",
                        "AngularVelocity",
                        "Mass",
                        "Mass.Center",
                        "Mass.RotationInertia",
                        "Inertia",
                        "LinearDamping",
                        "AngularDamping",
                        "Bullet",
                        "SleepingAllowed",
                        "Awake",
                        "Active",
                        "FixedRotation"
                    };
                    vector<string> members;
                    BOOST_FOREACH(string member, fields)
                    {
                        members.push_back(member);
                    }
                    answer += add_member_watch(watch, *it, members, targets, evalute_b2Body);
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
                    const int count = 5;
                    string fields[count] = {
                        "IsDefaultMaterial",
                        "MaxKindleLevel",
                        "MaxDampness",
                        "AcceptsCord",
                        "Name"
                    };
                    vector<string> members;
                    BOOST_FOREACH(string member, fields)
                    {
                        members.push_back(member);
                    }
                    answer += add_member_watch(watch, *it, members, targets, evalute_BodyPart);
                    watchNormal = true;
                }
                else
                {
                    //Body
                    answer += "Coming soon...\n";
                }
            }
            else
            {
                answer += "Error: unspecified 'of' parameter or it's argument.\n";
            }   
        }
        else if( command.find("environ") != command.npos)
        {
            answer += "Coming soon...";
        }
        else if( command.find("elapsed") != command.npos)
        {
            answer += "Coming soon...";
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
            answer += "Error: parametes not specified.\n";
        }
        answer += "Watch not added.\n";
    }
    return answer;
}



map<Target, string>& DebugWatcher::getTargets(StrIterator command, StrIterator end, TargetType type, string& answer)
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
                                if(partID != "")
                                {
                                    b2Fixture* fixture = getFixture(obj, &partID);
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
                    }//if((*it)->mName == objName)
                }//for(it = bodies.begin(); it != bodies.end(); ++it)
            }//if(argument != end)
            else
            {
                answer += "Error: argument for parameter 'of' not specified!\n";
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

string DebugWatcher::add_member_watch(Watch* watch, string command,
         vector<string>& members, map<Target, string>& targets, EvaluteFunction evalute)
{
    string answer = "";
    answer += "Selected " + IntToStr(targets.size()) + " targets.\n";
    if(targets.size() > 0)
    {
        watch->Active = false;
        addWatchToConsole(watch);
        map<Target, string>::iterator it;
        unsigned int startParamBraket = command.find("(");
        // If members not specified, add to watch all members
        if(startParamBraket == command.npos)
        {
            for(it = targets.begin(); it != targets.end(); ++it)
            {
                Target target = it->first;
                string watchName = it->second; 
                
                BOOST_FOREACH(string memberName, members)
                {
                    if(memberName.find(".") == memberName.npos)
                    {
                        Watch* child = new Watch(*watch);
                        child->Name = memberName;
                        child->Active = true;
                        child->Object = target;
                        child->Expression = evalute;
                        child->MemberName = memberName;
                        child->Parent = watch;
                        watch->Children.push_back(child);
                        child->ID = watch->ID + "." + watchName + "(" + memberName + ")";
                        mWatches.push_back(child);
                        addWatchToConsole(child);
                        answer += "Added member " + memberName + " for " + watchName + " target.\n";
                    }
                } 
            }
            answer += "All members added to watch.\n";
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
            string added = "";
            for(it = targets.begin(); it != targets.end(); ++it)
            {
                Target target = it->first;
                string watchName = it->second; 
                added = "";
                BOOST_FOREACH(string memberName, specMembers)
                {                   
                    boost::trim(memberName);  
                    
                    // Check, if member name is normal
                    if(find(members.begin(), members.end(), memberName) != members.end())
                    {
                        Watch* child = new Watch(*watch);
                        child->Name = memberName;
                        child->Active = true;
                        child->Object = target;
                        child->Expression = evalute;
                        child->MemberName = memberName;
                        child->Parent = watch;
                        watch->Children.push_back(child);
                        child->ID = watch->ID + "." + watchName + "(" + memberName + ")";
                        mWatches.push_back(child);
                        addWatchToConsole(child);
                        added += memberName + ", ";
                    }
                    else
                    {
                        answer += "Error: member " + memberName + " not exists.\n";
                    }
                }
            }
            //Remove last comma and space
            if( added.size() > 2) 
                added.erase(added.end() - 2, added.end());
            answer += "To watch added next properties of material: " + added + ". \n";
           
            answer += ". \n";
        }
    }//if(targets.size() > 0)
    else
    {
        watch->Type = NotAWatch;
    }


    return answer;
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
                    partID->assign(part->mName);
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
                if(part->mName == *partID)
                {
                    result = fixture;
                    partID->assign(part->mName);
                    break;
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
                BOOST_FOREACH(Watch* child, watch->Children)
                {
                    mWatches.remove(child);
                    removeWatchFromConsole(child);
                    removed++;
                    delete child;
                }
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
                BOOST_FOREACH(Watch* child, watch->Children)
                {
                    mWatches.remove(child);
                    removeWatchFromConsole(child);
                    removed++;
                    delete child;
                }
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
                BOOST_FOREACH(Watch* child, watch->Children)
                {
                    mWatches.remove(child);
                    removeWatchFromConsole(child);
                    removed++;
                    delete child;
                }
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
                    BOOST_FOREACH(Watch* child, watch->Children)
                    {
                        child->Active = false;
                        stopped++;
                    }
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
                    BOOST_FOREACH(Watch* child, watch->Children)
                    {
                        child->Active = false;
                        stopped++;
                    }
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
                    BOOST_FOREACH(Watch* child, watch->Children)
                    {
                        child->Active = false;
                        stopped++;
                    }
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
                next->Parent = watch;
                watch->Children.push_back(next);
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
















