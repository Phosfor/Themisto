/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Objects/PhysicObject.hpp"

void PhysicObject::setVisual(BodyVisual *visualiser) { mBodyVisual = visualiser; }
BodyVisual &PhysicObject::getVisual() { return *mBodyVisual; }
void PhysicObject::setBody(Body *body) { mBody = body; }
Body &PhysicObject::getBody() { return *mBody; }

PhysicObject::PhysicObject(Body* body, BodyVisual* visual)
{
    mBody = body;
    mBodyVisual = visual;
    mName = worldManager().generateUniqueID();
    mType = PhysicBodyObject;
    mShouldFreeBodyVisual = mShouldFreeBody = true;
}

PhysicObject::~PhysicObject()
{
    if(mShouldFreeBodyVisual && mBodyVisual != NULL) delete mBodyVisual; 
    if(mShouldFreeBody && mBody != NULL) delete mBody; 
}


void PhysicObject::updateVisual() 
{
    if(mBodyVisual != NULL)
    {
        if (mBodyVisual->getVisualState()) mBodyVisual->redrawBody(*mBody); 
    }
    else
    {
        throw new CL_Exception("BodyVisual not set.");
    }
}
void PhysicObject::step(float32 elapsed) 
{
    if(mBody != NULL)
    {
        mBody->step(elapsed); 
    }
    else
    {
        throw new CL_Exception("Body not set.");
    }
}

CL_Pointf PhysicObject::getPosition()
{
    b2Vec2 position = mBody->getb2Body()->GetPosition();
    return CL_Pointf(position.x, position.y);
}

void PhysicObject::update(float elapsed) 
{
    step(elapsed);
    updateVisual();
}

// -----------------------------------------------------------------
// ---------------------------- PARSING ----------------------------
// -----------------------------------------------------------------

Object* PhysicObject::ParsePhysicObject(CL_DomElement* tag, std::string& desc)
{
    using namespace boost;

    // Possible memory leack! Returning object without freeing it.
    // TODO: Replace with shared pointer
    BodyVisual* visualHandle = new BodyVisual();

    // Parsing visuals
    {
        CL_DomNodeList VisualTags = tag->get_elements_by_tag_name("Visual");
        if(VisualTags.get_length() > 1 /*|| VisualTags.get_length() == 0*/)
        {
            desc += "Error: One, and only one tag Visual must be in Object.";
            return NULL;
        }

        if (VisualTags.get_length() != 0)
        {
            CL_DomNodeList childList = VisualTags.item(0).get_child_nodes();
            for (int i=0; i < childList.get_length(); ++i)
            {
                CL_DomElement tag2 = childList.item(i).to_element();
                if (tag2.get_node_name() == "Position")
                {
                    visualHandle->mXPos = lexical_cast<float>(tag2.get_attribute("x").c_str());
                    visualHandle->mYPos = lexical_cast<float>(tag2.get_attribute("y").c_str());
                }
                if (tag2.get_node_name() == "Size")
                {
                    visualHandle->mSizeWidth = lexical_cast<float>(tag2.get_attribute("width").c_str());
                    visualHandle->mSizeHeight = lexical_cast<float>(tag2.get_attribute("height").c_str());
                }
                if (tag2.get_node_name() == "Texture")
                {
                    visualHandle->mSectionName = tag2.get_attribute("section").c_str();
                    visualHandle->mTextureName = tag2.get_attribute("name").c_str();
                }
            }

            visualHandle->setVisualState(true);
            visualHandle->configureVisual();
        }
        else
        {
            visualHandle->setVisualState(false);
        }
    }

   CL_DomNodeList BodyTags = tag->get_elements_by_tag_name("Body");
   if(BodyTags.get_length() > 1 || BodyTags.get_length() == 0)
   {
       desc += "Error: one and only one tag Body can appear in Object.";
       return NULL;
   }
   CL_DomElement body = BodyTags.item(0).to_element();

    // TODO: Make shared pointer here!!!
    // Physic body variables
    b2Body *b2body = NULL;
    b2BodyDef bdef;
    Body *bodyHandle = NULL;

    // b2Body tag
    CL_DomNodeList b2BodyTags = body.get_elements_by_tag_name("b2Body");
    if(b2BodyTags.get_length() > 1 || b2BodyTags.get_length() == 0)
    {
        desc += "Error: One, and only one tag b2Body must be in Body.";
        return NULL;
    }
    CL_DomElement b2BodyTag = b2BodyTags.item(0).to_element();
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

    b2body = physicManager().getWorld().CreateBody(&bdef);
    bodyHandle = new Body(b2body);

    // Go through all physic parts
    CL_DomNodeList parts = body.get_elements_by_tag_name("Part");
    for (int i=0; i < parts.get_length(); ++i)
    {
        b2FixtureDef fixdef;
        b2CircleShape cshape;
        b2PolygonShape pshape;

        //b2Vec2* normals = NULL;
        b2Fixture *fixture = NULL;
        BodyMaterial *materialHandle = NULL;
        BodyState *stateHandle = NULL;
        BodyPart *partHandle = NULL;
        b2Filter filter;

        // Parse b2Fixture
        CL_DomElement physicPart = parts.item(i).to_element();
        CL_DomNodeList b2FixtureTags = physicPart.get_elements_by_tag_name("b2Fixture");
        if(b2FixtureTags.get_length() > 1 || b2FixtureTags.get_length() == 0)
        {
            desc += "Error: one and only one b2Fixture tag can appear in Part.";
            return NULL;
        }
        else
        {
            CL_DomElement partChild = b2FixtureTags.item(0).to_element();
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
                    CL_DomNodeList TypeTags = fixtureParam.get_elements_by_tag_name("Type");
                    if(TypeTags.get_length() > 1 || TypeTags.get_length() == 0)
                    {
                        desc += "Error: one and only one tag Type can appear in Shape.";
                        return NULL;
                    }
                    CL_DomElement type = TypeTags.item(0).to_element();
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
                                b2Vec2 vertices[verticesList.get_length()];
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
                        }
                        fixdef.shape = &pshape;
                    }
                }
            }

            fixture = b2body->CreateFixture(&fixdef);
            partHandle = new BodyPart(fixture, worldManager().mDefaultMaterial);
        }

        // Go through Part children
        CL_DomNodeList physicPartChildren = physicPart.get_child_nodes();
        for (int i=0; i < physicPartChildren.get_length(); ++i)
        {
            CL_DomElement partChild = physicPartChildren.item(i).to_element();

            if(partChild.get_node_name() == "MaxKindleLevel")
            {
                float value = lexical_cast<float>(partChild.get_text().c_str());
                partHandle->setMaxKindleLevel(value);
            }
            else if(partChild.get_node_name() == "Name")
            {
                std::string value = partChild.get_text().c_str();
                partHandle->setName(value);
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
        if(matList.get_length() > 1)
        {
            desc += "Error: only one Material tag can apear in Part.";
            return NULL;
        }

        // The material tag doesn't exist
        if (matList.get_length() <= 0)
        {
            materialHandle = worldManager().mDefaultMaterial;
            partHandle->mShouldFreeBodyMaterial = false;
        }
        else
        {
            materialHandle = new BodyMaterial();
            partHandle->mShouldFreeBodyMaterial = true;
            CL_DomElement matElement = matList.item(0).to_element(); // item(0) is ok

            CL_DomNodeList NameTags = matElement.get_elements_by_tag_name("Name");
            if(NameTags.get_length() > 1)
            {
                desc += "Error: only one tag Name can appear in Material.";
                return NULL;
            }
            else if(NameTags.get_length() > 0)
            {
                materialHandle->Name = NameTags.item(0).to_element().get_text();
            }
            else
            {
                materialHandle->Name = worldManager().generateUniqueID();
            }

            CL_DomNodeList matChildList = matElement.get_child_nodes();
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

        partHandle->setMaterial(materialHandle, materialHandle == worldManager().mDefaultMaterial );
    } // for (int i=0; i < parts.get_length(); ++i)

    return new PhysicObject(bodyHandle, visualHandle);
}

