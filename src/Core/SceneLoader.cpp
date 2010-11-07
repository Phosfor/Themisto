#include "Core/SceneLoader.hpp"

/* TODO:
 * Parse hex
 * Write destructor and delete dynamic objects
 */

void SceneLoader::loadScene(const std::string &sceneName)
{
    //mThread.start<SceneLoader, const std::string&>(this, &SceneLoader::_threadWrapper, sceneName);
    //mThread.join();
    _threadWrapper(sceneName);
}

void SceneLoader::_threadWrapper(const std::string &sceneName)
{
    //mMutex.lock();

    CL_File fileHandle = CL_File("media/levels/" + sceneName);
    CL_DomDocument document(fileHandle);

    CL_DomElement root = document.get_document_element();
    CL_DomElement world = root.get_elements_by_tag_name("World").item(0).to_element();
    CL_DomElement environ = world.get_elements_by_tag_name("Environ").item(0).to_element();
    CL_DomElement objects = world.get_elements_by_tag_name("Objects").item(0).to_element();

    // Get level attributes
    CL_String authors = root.get_attribute("authors");
    CL_String version = root.get_attribute("version");

    LOG(cl_format("Loading new scene with name `%1` for the game version: `%2`. Created by: `%3`."
                  , sceneName, version, authors));

    // Check versions of the game and the level
    if (boost::lexical_cast<float>(version.c_str()) != GAME_VERSION)
        LOG(cl_format("Warning! Level version is different with game one(%1). Possible faults.", version));

    // Get world dimensions
    int worldWidth = boost::lexical_cast<int>(world.get_attribute("size_width").c_str());
    int worldHeight = boost::lexical_cast<int>(world.get_attribute("size_height").c_str());
    LOG_NOFORMAT(cl_format("- World size width: %1\n- World size height: %2\n", worldWidth, worldHeight));

    // Check whether environ should be enabled
    bool mEnvironEnabled = false;
    if (environ.get_attribute("active") == "true") mEnvironEnabled = true;
    environManager.setEnvironEnabled(mEnvironEnabled);
    LOG_NOFORMAT(cl_format("- Environ enabled state: %1\n", mEnvironEnabled));

    // If Environ is enabled, go through all environ-params in the level file
    if (mEnvironEnabled)
    {
        using namespace boost;
        environManager.initEnviron();

        std::map<std::string, EnvironTypes> deps;
        deps["Rain"] = Environ_Rain;
        deps["Clouds"] = Environ_Clouds;
        deps["Lightnings"] = Environ_Lightnings;
        deps["Sky"] = Environ_Sky;
        deps["Moon"] = Environ_Moon;
        deps["Leaves"] = Environ_Leaves;
        deps["Stars"] = Environ_Stars;
        deps["Birds"] = Environ_Birds;

        CL_DomNodeList childList = environ.get_child_nodes();
        for (int i=0; i < childList.get_length(); ++i)
        {
            CL_DomNode tag = childList.item(i);
            if (tag.get_node_name() == "Wind")
            {
                float pow = lexical_cast<float>(tag.to_element().get_attribute("power").c_str());
                environManager.setWindPower(pow);
            }
            EnvironTypes type = deps[tag.get_node_name().c_str()];

            bool enabled = tag.to_element().get_attribute("enabled") == "true";

            int lim = -1;
            CL_String s_lim = tag.to_element().get_attribute("limit");
            if (s_lim != "") lim = lexical_cast<int>(s_lim.c_str());

            environManager.enableType(enabled, type, lim);
        }
        LOG_NOFORMAT("- All environ objects are loaded.\n");
    }

    // START OF OBJECTS PARSING -------------------------------------------
    {
        using namespace boost;

        float x = 0, y = 0;

        CL_DomNodeList childList = objects.get_child_nodes();
        for (int i=0; i < childList.get_length(); ++i)
        {
            CL_DomElement tag = childList.item(i).to_element();

            std::string name = tag.get_attribute("name").c_str();
            std::string type = tag.get_attribute("type").c_str();
            bool physic = tag.get_attribute("physic_body") == "true";
            LOG_NOFORMAT(cl_format("- Parsing object `%1` of type `%2`, physic body: `%3`\n", 
                        name, type, physic));

            // Parsing visuals
            {
                CL_DomElement visual = tag.get_elements_by_tag_name("Visual").item(0).to_element();
                CL_DomNodeList childList = visual.get_child_nodes();

                for (int i=0; i < childList.get_length(); ++i)
                {
                    CL_DomElement tag2 = childList.item(i).to_element();
                    if (tag2.get_node_name() == "Position")
                    {
                        x = lexical_cast<float>(tag2.get_attribute("x").c_str());
                        y = lexical_cast<float>(tag2.get_attribute("y").c_str());
                    }
                }
            }

            // START OF PHYSIC PARSING -------------------------------------------
            {
                // Physic body variables
                b2BodyDef bdef;
                b2FixtureDef fixdef;
                b2Fixture *fixture;
                b2Filter filter;
                b2Body *b2body;
                Body *bodyHandle;
                BodyPart *partHandle;

                CL_DomElement body = tag.get_elements_by_tag_name("Body").item(0).to_element();

                // b2Body tag
                CL_DomElement b2BodyTag = body.get_elements_by_tag_name("b2Body").item(0).to_element();
                CL_DomNodeList b2BodyTagList = b2BodyTag.get_child_nodes();
                for (int i=0; i < b2BodyTagList.get_length(); ++i)
                {
                    // Get handle of the tag
                    CL_DomElement child = b2BodyTagList.item(i).to_element();

                    // Position parsing
                    if (child.get_node_name() == "Position")
                    {
                        float x = 0, y = 0;
                        CL_DomNodeList position = child.get_child_nodes();
                        for (int i=0; i < position.get_length(); ++i)
                        {
                            CL_DomElement handle = position.item(i).to_element();
                            if (handle.get_node_name() == "x")
                                x = lexical_cast<float>(handle.get_text().c_str());
                            else if (handle.get_node_name() == "y")
                                y = lexical_cast<float>(handle.get_text().c_str());
                        }
                        bdef.position.Set(x, y);
                    }

                    // Type parsing
                    if (child.get_node_name() == "Type")
                    {
                        std::string type = child.get_text().c_str();
                        if (type == "StaticBody")
                            bdef.type = b2_staticBody;
                        else if (type == "KinematicBody")
                            bdef.type = b2_kinematicBody;
                        else if (type == "DynamicBody")
                            bdef.type = b2_dynamicBody;
                    }

                    // Angle parsing
                    if (child.get_node_name() == "Angle")
                    {
                        float angle = lexical_cast<float>(child.get_text().c_str());
                        // TODO: SET ANGLE SOMEWHERE !!!!!!11111адынадын
                    }

                    // Linear velocity parsing
                    if (child.get_node_name() == "LinearVelocity")
                    {
                        float x = 0, y = 0;
                        CL_DomNodeList velocity = child.get_child_nodes();
                        for (int i=0; i < velocity.get_length(); ++i)
                        {
                            CL_DomElement handle = velocity.item(i).to_element();
                            if (handle.get_node_name() == "x")
                                x = lexical_cast<float>(handle.get_text().c_str());
                            else if (handle.get_node_name() == "y")
                                y = lexical_cast<float>(handle.get_text().c_str());
                        }
                        bdef.linearVelocity.Set(x, y);
                    }

                    // Angular velocity parsing
                    if (child.get_node_name() == "AngularVelocity")
                    {
                        float angle = lexical_cast<float>(child.get_text().c_str());
                        bdef.angularVelocity = angle;
                    }

                    // Linear Damping parsing
                    if (child.get_node_name() == "LinearDamping")
                    {
                        float value = lexical_cast<float>(child.get_text().c_str());
                        bdef.linearDamping = value;
                    }

                    // Allow sleep parsing
                    if (child.get_node_name() == "AllowSleep")
                    {
                        bool value = child.get_text() == "true";
                        bdef.allowSleep = value;
                    }

                    // Awake parsing
                    if (child.get_node_name() == "Awake")
                    {
                        bool value = child.get_text() == "true";
                        bdef.awake = value;
                    }

                    // Fixed rotation parsing
                    if (child.get_node_name() == "FixedRotation")
                    {
                        bool value = child.get_text() == "true";
                        bdef.fixedRotation = value;
                    }

                    // Bullet parsing
                    if (child.get_node_name() == "Bullet")
                    {
                        bool value = child.get_text() == "true";
                        bdef.bullet = value;
                    }

                    // Active parsing
                    if (child.get_node_name() == "Active")
                    {
                        bool value = child.get_text() == "true";
                        bdef.active = value;
                    }
                }

                b2body = physicManager.getWorld().CreateBody(&bdef);
                bodyHandle = new Body(b2body);
                physicManager.registerBody(bodyHandle);

                // Go through all physic parts
                CL_DomNodeList parts = body.get_elements_by_tag_name("Part");
                for (int i=0; i < parts.get_length(); ++i)
                {
                    CL_DomElement physicPart = parts.item(i).to_element();

                    // Go through Part children
                    CL_DomNodeList physicPartChildren = physicPart.get_child_nodes();
                    for (int i=0; i < physicPartChildren.get_length(); ++i)
                    {
                        CL_DomElement partChild = physicPartChildren.item(i).to_element();
                        if (partChild.get_node_name() == "b2Fixture")
                        {
                            // Go through fixture params list
                            CL_DomNodeList fixtureParamList = partChild.get_child_nodes();
                            for (int i=0; i < fixtureParamList.get_length(); ++i)
                            {
                                CL_DomElement fixtureParam = fixtureParamList.item(i).to_element();

                                // Parse fixture friction value
                                if (fixtureParam.get_node_name() == "Friction")
                                {
                                    float value = lexical_cast<float>(fixtureParam.get_text().c_str());
                                    fixdef.friction = value;
                                }

                                // Parse restitution value
                                if (fixtureParam.get_node_name() == "Restitution")
                                {
                                    float value = lexical_cast<float>(fixtureParam.get_text().c_str());
                                    fixdef.restitution = value;
                                }

                                // Parse density value
                                if (fixtureParam.get_node_name() == "Density")
                                {
                                    float value = lexical_cast<float>(fixtureParam.get_text().c_str());
                                    fixdef.density = value;
                                }

                                // Parse sensor value
                                if (fixtureParam.get_node_name() == "IsSensor")
                                {
                                    bool value = fixtureParam.get_text() == "true";
                                    fixdef.isSensor = value;
                                }

                                // Parse filter params
                                if (fixtureParam.get_node_name() == "Filter")
                                {
                                    CL_DomNodeList filterList = fixtureParam.get_child_nodes();
                                    for (int i=0; i < filterList.get_length(); ++i)
                                    {
                                        CL_DomElement filterParam = filterList.item(i).to_element();
                                        if (filterParam.get_node_name() == "CategoryBits")
                                        {
                                            std::string value = filterParam.get_text().c_str();
                                            // TODO: Parse hex-value
                                            // filter.categoryBits = uint16
                                        }

                                        if (filterParam.get_node_name() == "MaskBits")
                                        {
                                            std::string value = filterParam.get_text().c_str();
                                            // TODO: Parse hex-value
                                            // filter.maskBits = uint16
                                        }
                                        if (filterParam.get_node_name() == "GroupIndex")
                                        {
                                            int value = lexical_cast<int>(filterParam.get_text().c_str());
                                            filter.groupIndex = value;
                                        }
                                    }

                                    // Set up filter for the fixture object
                                    fixdef.filter = filter;
                                }

                                // Parse body shape
                                if (fixtureParam.get_node_name() == "Shape")
                                {
                                    b2Shape::Type typeHandle;

                                    // Parse shape type
                                    CL_DomElement type = fixtureParam.get_elements_by_tag_name("Type").item(0).to_element();
                                    if (type.get_text() == "e_circle")
                                        typeHandle = b2Shape::e_circle;
                                    else if(type.get_text() == "e_polygon")
                                        typeHandle = b2Shape::e_polygon;

                                    // The shape is a circle
                                    if (typeHandle == b2Shape::e_circle)
                                    {
                                        float radius = 0, x = 0, y = 0;
                                        CL_DomNodeList shapeList = fixtureParam.get_child_nodes();
                                        for (int i=0; i < shapeList.get_length(); ++i)
                                        {
                                            CL_DomElement shapeParam = shapeList.item(i).to_element();
                                            if (shapeParam.get_node_name() == "Radius")
                                            {
                                                radius = lexical_cast<float>(shapeParam.get_text().c_str());
                                            }
                                            else if(shapeParam.get_node_name() == "Center")
                                            {
                                                CL_DomNodeList centerList = shapeParam.get_child_nodes();
                                                for (int i=0; i < centerList.get_length(); ++i)
                                                {
                                                    CL_DomElement centerCoord = centerList.item(i).to_element();
                                                    if (centerCoord.get_node_name() == "x")
                                                        x = lexical_cast<float>(centerCoord.get_text().c_str());
                                                    if(centerCoord.get_node_name() == "y")
                                                        y = lexical_cast<float>(centerCoord.get_text().c_str());
                                                }
                                            }
                                        }

                                        b2CircleShape *shapeHandle = new b2CircleShape();
                                        shapeHandle->m_radius = radius;
                                        shapeHandle->m_p.Set(x, y);
                                        fixdef.shape = shapeHandle;
                                    }
                                    // The shape is a polygon
                                    else if (typeHandle == b2Shape::e_polygon)
                                    {
                                        std::vector<float> verticesListX, verticesListY, normalsListX, normalsListY;
                                        float radius = 0, x = 0, y = 0;

                                        CL_DomNodeList shapeList = fixtureParam.get_child_nodes();
                                        for (int i=0; i < shapeList.get_length(); ++i)
                                        {
                                            CL_DomElement shapeParam = shapeList.item(i).to_element();
                                            if(shapeParam.get_node_name() == "Center")
                                            {
                                                CL_DomNodeList centerList = shapeParam.get_child_nodes();
                                                for (int i=0; i < centerList.get_length(); ++i)
                                                {
                                                    CL_DomElement centerCoord = centerList.item(i).to_element();
                                                    if (centerCoord.get_node_name() == "x")
                                                        x = lexical_cast<float>(centerCoord.get_text().c_str());
                                                    if(centerCoord.get_node_name() == "y")
                                                        y = lexical_cast<float>(centerCoord.get_text().c_str());
                                                }
                                            }
                                            // Parse polygon vertices
                                            else if(shapeParam.get_node_name() == "Vertices")
                                            {
                                                CL_DomNodeList verticesList = shapeParam.get_child_nodes();
                                                for (int i=0; i < verticesList.get_length(); ++i)
                                                {
                                                    CL_DomElement vertex = verticesList.item(i).to_element();
                                                    if (vertex.get_node_name() == "Vertex")
                                                    {
                                                        CL_DomNodeList vertexComponents = vertex.get_child_nodes();
                                                        for (int j=0; j < vertexComponents.get_length(); ++j)
                                                        {
                                                            CL_DomElement component = vertexComponents.item(j).to_element();
                                                            if (component.get_node_name() == "x")
                                                            {
                                                                float x = lexical_cast<float>(component.get_text().c_str());
                                                                verticesListX.push_back(x);
                                                            }
                                                            else if(component.get_node_name() == "y")
                                                            {
                                                                float y = lexical_cast<float>(component.get_text().c_str());
                                                                verticesListY.push_back(y);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            // Parse polygon normals
                                            else if (shapeParam.get_node_name() == "Normals")
                                            {
                                                CL_DomNodeList normalsList = shapeParam.get_child_nodes();
                                                for (int i=0; i < normalsList.get_length(); ++i)
                                                {
                                                    CL_DomElement normal = normalsList.item(i).to_element();
                                                    if (normal.get_node_name() == "Normal")
                                                    {
                                                        CL_DomNodeList normalComponents = normal.get_child_nodes();
                                                        for (int j=0; j < normalComponents.get_length(); ++j)
                                                        {
                                                            CL_DomElement component = normalComponents.item(j).to_element();
                                                            if (component.get_node_name() == "x")
                                                            {
                                                                float x = lexical_cast<float>(component.get_text().c_str());
                                                                normalsListX.push_back(x);
                                                            }
                                                            else if(component.get_node_name() == "y")
                                                            {
                                                                float y = lexical_cast<float>(component.get_text().c_str());
                                                                normalsListY.push_back(y);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        b2PolygonShape *shapeHandle = new b2PolygonShape();
                                        for (int i=0; i < verticesListX.size(); ++i)
                                            shapeHandle->m_vertices[i].Set(verticesListX[i], verticesListY[i]);
                                        shapeHandle->m_vertexCount = verticesListX.size();

                                        for (int i=0; i < normalsListX.size(); ++i)
                                            shapeHandle->m_normals[i].Set(normalsListX[i], normalsListY[i]);
                                        shapeHandle->m_centroid.Set(x, y);
                                        fixdef.shape = shapeHandle;
                                    }
                                }
                            }

                            fixture = b2body->CreateFixture(&fixdef);
                            partHandle = new BodyPart(fixture);
                        }
                        else if(partChild.get_node_name() == "MaxKindleLevel")
                        {
                            float value = lexical_cast<float>(partChild.get_text().c_str());
                            partHandle->mMaxKindleLevel = value;
                        }
                    }
                }
            }
            // END OF PHYSIC PARSING ---------------------------------------------

            // Creating final object
            Object *obj = BuildObjectType(type);
            obj->setPosition(CL_Pointf(x, y));
            obj->setName(name);
            objectsManager.addObject(name, obj);
        }
    }
    // END OF OBJECTS PARSING -------------------------------------------

    //mMutex.unlock();
}
