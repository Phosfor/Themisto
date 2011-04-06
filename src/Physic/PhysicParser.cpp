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

#include "Core/WorldManager.hpp"
#include "Core/Utils.hpp"
#include "Core/PhysicManager.hpp"
#include "Physic/Impact.hpp"
#include "Physic/Body.hpp"
#include "World/Objects/Object.hpp"


boost::shared_ptr<Body> ParsePhysicBody(CL_DomElement body)
{
    using namespace boost;

    boost::shared_ptr<Body> bodyHandle = boost::shared_ptr<Body>(new Body());;
    b2Body *b2body = NULL;

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
    bodyHandle->setBody(b2body);

    // Parse b2Fixture
    CL_DomNodeList b2FixtureTags = body.get_elements_by_tag_name("b2Fixture");
    for(int fixtureInd = 0; fixtureInd < b2FixtureTags.get_length(); ++fixtureInd)
    {
        b2FixtureDef fixdef;
        b2CircleShape cshape;
        b2PolygonShape pshape;
        b2Filter filter;

        CL_DomElement partChild = b2FixtureTags.item(fixtureInd).to_element();
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

        b2body->CreateFixture(&fixdef);
    }

    // Parse dynamic properties
    CL_DomNodeList PropertiesTags = body.get_elements_by_tag_name("Properties");
    if(PropertiesTags.get_length() > 1)
    {
        throw CL_Exception("Only one tag 'Properties' may be in Body");
    }
    else if(PropertiesTags.get_length() == 1)
    {
        CL_DomElement propertiesTag = PropertiesTags.item(0).to_element();
        CL_DomNodeList properties = propertiesTag.get_child_nodes();
        for(int i = 0; i < properties.get_length(); ++i)
        {
            if(properties.item(i).is_element())
            {
                CL_DomElement property = properties.item(i).to_element();
                bodyHandle->setProperty(property.get_node_name().c_str(), property.get_text().c_str());
            }
        }
    }

    return bodyHandle;
}
