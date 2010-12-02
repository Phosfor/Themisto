/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Objects/Body.hpp"

Body::Body(b2Body* body)
{
    mBody = body;
    
    mType = PhysicBodyObject;
    body->SetUserData(this);
    mBodyVisual = NULL;
    mShouldFreeBodyVisual = true;
    mShouldFreeB2Body = true;
    mParentWorld= mBody->GetWorld();
    mLastLocation = calculateLocation();
    world = &worldManager;
    mName = worldManager.generateUniqueID();
}


Body::~Body()
{
    if(mShouldFreeBodyVisual) if(mBodyVisual != NULL) delete mBodyVisual;
    if(mShouldFreeB2Body) mParentWorld->DestroyBody(mBody);
}

CL_Pointf Body::getPosition()
{
    b2Vec2 position = mBody->GetPosition();
    return CL_Pointf(position.x, position.y);
}

void Body::setVisual(BodyVisual* visualiser)
{
    mBodyVisual = visualiser;
}

b2Body* Body::getb2Body()
{
    return mBody;
}

void Body::updateVisual()
{
    if(mBodyVisual != NULL)
    {
        mBodyVisual->redrawBody();
    }
}


void Body::step(float32 elapsed)
{
    
    for(b2Fixture* fixture = mBody->GetFixtureList();
            fixture != NULL; fixture = fixture->GetNext())
    {
        BodyPart* part = (BodyPart*)fixture->GetUserData();
        if( part != NULL)
            part->step(elapsed);
    }

    b2AABB currentLocation = calculateLocation();
    
    b2Vec2 ll = mLastLocation.lowerBound;
    b2Vec2 cl = currentLocation.lowerBound;
    float deltaX = cl.x - ll.x;
    float deltaY = cl.y - ll.y;
    float cellSize = areaManager.getCellSize();
    // If body moved father then cell size
    if( abs(deltaX) > cellSize ||  abs(deltaY) > cellSize)
    {
        for(b2Fixture* fixture = mBody->GetFixtureList();
            fixture != NULL; fixture = fixture->GetNext())
        {
            b2AABB lastLocation = fixture->GetAABB();
            b2Vec2 delta(deltaX, deltaY);
            lastLocation.lowerBound = lastLocation.lowerBound - delta;
            lastLocation.upperBound = lastLocation.upperBound - delta;
            areaManager.reportNewFixtureLocation(&lastLocation, 
                    &fixture->GetAABB(), fixture);
        }
        mLastLocation = currentLocation;
    }
}

b2AABB Body::calculateLocation()
{
    b2AABB region;
    b2Fixture* firstFixture = mBody->GetFixtureList();
    if(firstFixture != NULL)
    {
        b2AABB r = firstFixture->GetAABB();
        region.lowerBound = r.lowerBound;
        region.upperBound = r.upperBound;

        for(b2Fixture* fixture = firstFixture->GetNext();
                fixture != NULL; fixture = fixture->GetNext())
        {
            region.Combine(region, fixture->GetAABB());
        }
    }
    else
    {
        region.lowerBound.Set(0,0);
        region.upperBound.Set(0,0);
    }
    return region;
}



// ----------------------------------------------------------------
//---------------------------- PARSING-----------------------------
//----------------------------------------------------------------

Object* Body::ParseBody(CL_DomElement* tag)
{  
    float x = 0, y = 0;
    // Parsing visuals
    {
        CL_DomElement visual = tag->get_elements_by_tag_name("Visual").item(0).to_element();
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
    
           
   CL_DomElement body = tag->get_elements_by_tag_name("Body").item(0).to_element();

    // Physic body variables
    b2Body *b2body;
    b2BodyDef bdef;
    Body *bodyHandle;    

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
            bdef.angle = angle;
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
    

    // Go through all physic parts
    CL_DomNodeList parts = body.get_elements_by_tag_name("Part");
    for (int i=0; i < parts.get_length(); ++i)
    {
        b2FixtureDef fixdef;
        b2CircleShape cshape;
        b2PolygonShape pshape;
        b2Vec2* vertices = NULL;
        //b2Vec2* normals = NULL;
        b2Fixture *fixture;
        BodyMaterial *materialHandle;
        BodyState *stateHandle;
        BodyPart *partHandle;
        b2Filter filter;

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

                            cshape.m_radius = radius;
                            cshape.m_p.Set(x, y);
                            fixdef.shape = &cshape;
                        }
                        // The shape is a polygon
                        else if (typeHandle == b2Shape::e_polygon)
                        {

                            CL_DomNodeList shapeList = fixtureParam.get_child_nodes();
                            for (int i=0; i < shapeList.get_length(); ++i)
                            {
                                CL_DomElement shapeParam = shapeList.item(i).to_element();
                                if(shapeParam.get_node_name() == "Center")
                                {
                                    CL_DomNodeList centerList = shapeParam.get_child_nodes();
                                    for (int i=0; i < centerList.get_length(); ++i)
                                    {
                                        float x = 0, y = 0;
                                        CL_DomElement centerCoord = centerList.item(i).to_element();
                                        if (centerCoord.get_node_name() == "x")
                                            x = lexical_cast<float>(centerCoord.get_text().c_str());
                                        if(centerCoord.get_node_name() == "y")
                                            y = lexical_cast<float>(centerCoord.get_text().c_str());
                                        pshape.m_centroid.Set(x,y);
                                    }
                                }
                                // Parse polygon vertices
                                else if(shapeParam.get_node_name() == "Vertices")
                                {
                                    CL_DomNodeList verticesList = shapeParam.get_child_nodes();
                                    vertices = new b2Vec2[verticesList.get_length()];
                                    for (int i=0; i < verticesList.get_length(); ++i)
                                    {
                                        CL_DomElement vertex = verticesList.item(i).to_element();
                                        if (vertex.get_node_name() == "Vertex")
                                        {
                                            CL_DomNodeList vertexComponents = vertex.get_child_nodes();
                                            float x = 0, y = 0;
                                            for (int j=0; j < vertexComponents.get_length(); ++j)
                                            {
                                                CL_DomElement component = vertexComponents.item(j).to_element();
                                                if (component.get_node_name() == "x")
                                                {
                                                    x = lexical_cast<float>(component.get_text().c_str());
                                                }
                                                else if(component.get_node_name() == "y")
                                                {
                                                    y = lexical_cast<float>(component.get_text().c_str());
                                                }
                                            }
                                            vertices[i].Set(x,y);
                                        }
                                    }
                                    pshape.Set(vertices,verticesList.get_length());
                                }
                                // Parse polygon normals
                                /*else if (shapeParam.get_node_name() == "Normals")*/
                                //{
                                    //CL_DomNodeList normalsList = shapeParam.get_child_nodes();
                                    //normals = new b2Vec2[normalsList.get_length()];
                                    //for (int i=0; i < normalsList.get_length(); ++i)
                                    //{
                                        //CL_DomElement normal = normalsList.item(i).to_element();
                                        //if (normal.get_node_name() == "Normal")
                                        //{
                                            //CL_DomNodeList normalComponents = normal.get_child_nodes();
                                            //for (int j=0; j < normalComponents.get_length(); ++j)
                                            //{
                                                //float x=0, y=0;
                                                //CL_DomElement component = normalComponents.item(j).to_element();
                                                //if (component.get_node_name() == "x")
                                                //{
                                                    //x = lexical_cast<float>(component.get_text().c_str());
                                                //}
                                                //else if(component.get_node_name() == "y")
                                                //{
                                                    //y = lexical_cast<float>(component.get_text().c_str());
                                                //}
                                                //normals[i].Set(x,y);
                                            //}
                                        //}
                                    //}
                                    //pshape.m_normals = normals;
                                /*}*/
                            }

                            //for (unsigned int i=0; i < normalsListX.size(); ++i)
                                //pshape.m_normals[i].Set(normalsListX[i], normalsListY[i]);
                            fixdef.shape = &pshape;
                        }
                    }
                }
                
                fixture = b2body->CreateFixture(&fixdef);
                if(vertices != NULL) delete vertices;
                
                partHandle = new BodyPart(fixture, worldManager.mDefaultMaterial);
            }
            else if(partChild.get_node_name() == "MaxKindleLevel")
            {
                float value = lexical_cast<float>(partChild.get_text().c_str());
                partHandle->setMaxKindleLevel(value);
            }
            else if(partChild.get_node_name() == "MaxDampness")
            {
                float value = lexical_cast<float>(partChild.get_text().c_str());
                partHandle->setMaxDampness(value);
            }
            else if(partChild.get_node_name() == "AcceptsCord")
            {
                bool value = partChild.get_text() == "true";
                partHandle->setAcceptsCord(value);
            }
            
            else if(partChild.get_node_name() == "State")
            {
                stateHandle = partHandle->getState();
                CL_DomNodeList stateChildList = partChild.get_child_nodes();
                for (int i=0; i < stateChildList.get_length(); ++i)
                {
                    CL_DomElement stateElement = stateChildList.item(i).to_element();
                    if (stateElement.get_node_name() == "Dampness")
                    {
                        float value = lexical_cast<float>(stateElement.get_text().c_str());
                        stateHandle->Dampness = value;
                    }
                    else if (stateElement.get_node_name() == "KindleLevel")
                    {
                        float value = lexical_cast<float>(stateElement.get_text().c_str());
                        stateHandle->KindleLevel = value;
                    }
                    else if (stateElement.get_node_name() == "Temperature")
                    {
                        float value = lexical_cast<float>(stateElement.get_text().c_str());
                        stateHandle->Temperature = value;
                    }
                    else if (stateElement.get_node_name() == "CarbonizeLevel")
                    {
                        float value = lexical_cast<float>(stateElement.get_text().c_str());
                        stateHandle->CarbonizeLevel = value;
                    }
                    else if (stateElement.get_node_name() == "IsFrozen")
                    {
                        //bool value = lexical_cast<bool>(stateElement.get_text().c_str());
                        //stateHandle->IsFrozen = value;
                    }
                }
            }
            else if (partChild.get_node_name() == "StaticImpacts")
            {
                /*Impact* impact = new Impact();*/
                //CL_DomNodeList impactsList = partChild.get_child_nodes();
                //for (int i=0; i < impactsList.get_length(); ++i)
                //{
                    //CL_DomElement impact = impactsList.item(i).to_element();
                    //if (impact.get_node_name() == "Type")
                /*}*/
            }
        }
        
        // Parse material
        CL_DomNodeList matList = physicPart.get_elements_by_tag_name("Material");

        // The material tag doesn't exist
        if (matList.get_length() <= 0)
        {
            materialHandle = worldManager.mDefaultMaterial;
            partHandle->mShouldFreeBodyMaterial = false;
        }
        else
        {
            materialHandle = new BodyMaterial();
            partHandle->mShouldFreeBodyMaterial = true;
            materialHandle->Name = matList.item(0).to_element().get_child_string("Name");

            CL_DomNodeList matChildList = matList.item(0).get_child_nodes();
            // Go through material child tags
            for (int i=0; i < matChildList.get_length(); ++i)
            {
                CL_DomElement matChild = matChildList.item(i).to_element();
                if (matChild.get_node_name() == "KindleTemperature")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->KindleTemperature = value;
                }
                else if (matChild.get_node_name() == "KindleReceptivity")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->KindleReceptivity = value;
                }
                else if (matChild.get_node_name() == "FlameTemperature")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->FlameTemperature = value;
                }
                else if (matChild.get_node_name() == "SelfFlareUpRate")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->SelfFlareUpRate = value;
                }
                else if (matChild.get_node_name() == "CarbonizeRate")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->CarbonizeRate = value;
                }
                else if (matChild.get_node_name() == "ElectricalConductivity")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->ElectricalConductivity = value;
                }
                else if (matChild.get_node_name() == "ThermalReceptivity")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->ThermalReceptivity = value;
                }
                else if (matChild.get_node_name() == "DampReceptivity")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->DampReceptivity = value;
                }
                else if (matChild.get_node_name() == "FrozingTemperature")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->FrozingTemperature = value;
                }
                else if (matChild.get_node_name() == "InflDampnessToFriction")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflDampnessToFriction = value;
                }
                else if (matChild.get_node_name() == "InflDampnessToKindleTemperature")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflDampnessToKindleTemperature = value;
                }
                else if (matChild.get_node_name() == "InflDampnessToMaxKindle")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflDampnessToMaxKindle = value;
                }
                else if (matChild.get_node_name() == "InflDampnessToKindleReceptivity")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflDampnessToKindleReceptivity = value;
                }
                else if (matChild.get_node_name() == "InflDampnessToFrozingTemperature")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflDampnessToFrozingTemperature = value;
                }
                else if (matChild.get_node_name() == "InflCarbonizeLevelToMaxKindle")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflCarbonizeLevelToMaxKindle = value;
                }
                else if (matChild.get_node_name() == "InflCarbonizeLevelToMaxDampness")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflCarbonizeLevelToMaxDampness = value;
                }
                else if (matChild.get_node_name() == "InflCarbonizeLevelToElecticalConductivity")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflCarbonizeLevelToElecticalConductivity = value;
                }
                else if (matChild.get_node_name() == "InflMoistenToKindleLevel")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflMoistenToKindleLevel = value;
                }
                else if (matChild.get_node_name() == "InflTemperatureToDampness")
                {
                    float value = lexical_cast<float>(matChild.get_text().c_str());
                    materialHandle->InflTemperatureToDampness = value;
                }
            } // for (int i=0; i < matChildList.get_length(); ++i)
        } // if (matList.get_length() <= 0)

        partHandle->setMaterial(materialHandle, materialHandle == worldManager.mDefaultMaterial );
    } // for (int i=0; i < parts.get_length(); ++i)
    return bodyHandle;
}















