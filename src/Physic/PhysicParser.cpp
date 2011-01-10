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

#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <ClanLib/core.h>
#include <boost/lexical_cast.hpp>

#include "World/WorldManager.hpp"
#include "World/Objects/Object.hpp"
#include "Physic/BodyPart.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/Impact.hpp"
#include "Physic/BodyMaterial.hpp"
#include "Core/Utils.hpp"
#include "Core/PhysicManager.hpp"


std::pair< b2Body*, std::vector< boost::shared_ptr<BodyPart> > > ParsePhysicBody(CL_DomElement body, std::string& desc)
{
   using namespace boost;

   b2Body *b2body = NULL;
   std::vector< boost::shared_ptr<BodyPart> > bodyParts;

   if(body.get_node_name() != "Body")
   {
       throw CL_Exception("Error: invalid parameter 'tag' (node name not Body)");
   }

    b2BodyDef bdef;

    // b2Body tag
    CL_DomNodeList b2BodyTags = body.get_elements_by_tag_name("b2Body");
    if(b2BodyTags.get_length() > 1 || b2BodyTags.get_length() == 0)
    {
        throw CL_Exception("Error: One, and only one tag b2Body must be in Body.");
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
                    x = Pixels2Meters(lexical_cast<float>(handle.get_text().c_str()));
                else if (handle.get_node_name() == "y")
                    y = Pixels2Meters(lexical_cast<float>(handle.get_text().c_str()));
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

    LOG_NOFORMAT("\tParsed parts: ");
    // Go through all physic parts
    CL_DomNodeList parts = body.get_elements_by_tag_name("Part");
    for (int i=0; i < parts.get_length(); ++i)
    {
        b2FixtureDef fixdef;
        b2CircleShape cshape;
        b2PolygonShape pshape;

        //b2Vec2* normals = NULL;
        b2Fixture *fixture = NULL;
        boost::shared_ptr<BodyMaterial> materialHandle;
        boost::shared_ptr<BodyState> stateHandle;
        boost::shared_ptr<BodyPart> partHandle;
        b2Filter filter;

        LOG_NOFORMAT(cl_format("%1; ", parts.item(i).to_element().get_attribute("id")).c_str());

        // Parse b2Fixture
        CL_DomElement physicPart = parts.item(i).to_element();
        CL_DomNodeList b2FixtureTags = physicPart.get_elements_by_tag_name("b2Fixture");
        if(b2FixtureTags.get_length() > 1 || b2FixtureTags.get_length() == 0)
        {
            throw CL_Exception("Error: one and only one b2Fixture tag can appear in Part.");

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
                        throw CL_Exception("Error: one and only one tag Type can appear in Shape.");
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
                                radius = Pixels2Meters(lexical_cast<float>(shapeParam.get_text().c_str()));
                            }
                            else if(shapeParam.get_node_name() == "Center")
                            {
                                CL_DomNodeList centerList = shapeParam.get_child_nodes();
                                for (int i=0; i < centerList.get_length(); ++i)
                                {
                                    CL_DomElement centerCoord = centerList.item(i).to_element();
                                    if (centerCoord.get_node_name() == "x")
                                        x = Pixels2Meters(lexical_cast<float>(centerCoord.get_text().c_str()));
                                    if(centerCoord.get_node_name() == "y")
                                        y = Pixels2Meters(lexical_cast<float>(centerCoord.get_text().c_str()));
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
                                        x = Pixels2Meters(lexical_cast<float>(centerCoord.get_text().c_str()));
                                    if(centerCoord.get_node_name() == "y")
                                        y = Pixels2Meters(lexical_cast<float>(centerCoord.get_text().c_str()));
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
                                                x = Pixels2Meters(lexical_cast<float>(component.get_text().c_str()));
                                            }
                                            else if(component.get_node_name() == "y")
                                            {
                                                y = Pixels2Meters(lexical_cast<float>(component.get_text().c_str()));
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
            partHandle = boost::shared_ptr<BodyPart>(
                new BodyPart(fixture, worldManager().mDefaultMaterial));
            bodyParts.push_back(partHandle);
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
            throw CL_Exception("Error: only one Material tag can apear in Part.");
        }

        // The material tag doesn't exist
        if (matList.get_length() <= 0)
        {
            materialHandle = worldManager().mDefaultMaterial;
        }
        else
        {
            materialHandle = boost::shared_ptr<BodyMaterial>(new BodyMaterial());
            CL_DomElement matElement = matList.item(0).to_element(); // item(0) is ok

            CL_DomNodeList NameTags = matElement.get_elements_by_tag_name("Name");
            if(NameTags.get_length() > 1)
            {
                throw CL_Exception("Error: only one tag Name can appear in Material.");
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

        partHandle->setMaterial(materialHandle);
    } // for (int i=0; i < parts.get_length(); ++i)

    LOG_NOFORMAT("\n");

    return std::make_pair(b2body, bodyParts);
}
