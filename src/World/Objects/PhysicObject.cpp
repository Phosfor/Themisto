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

#include "World/Objects/PhysicObject.hpp"
#include "World/Actions/TestAction.hpp"
#include "Physic/Body.hpp"
#include "Physic/Impact.hpp"

PhysicObject::PhysicObject()
{
    mName = worldManager().generateUniqueID();
    mGC = appManager().getGraphic();
}

void PhysicObject::init() {}

void PhysicObject::setVisual(std::string textureName, std::string textureSection)
{
    setVisual(textureName, textureSection, -1, -1);
}

void PhysicObject::setVisual(std::string textureName, std::string textureSection, float width, float height)
{
    mImageHandle = resourceManager().getSprite(textureSection, textureName);
    if(!mImageHandle.is_null())
    {
        float koefX = 1;
        if(width > 0)
        {
            koefX = width / mImageHandle.get_width();
        }
        float koefY = 1;
        if(height > 0)
        {
            koefY = height / mImageHandle.get_height();
        }
        mImageHandle.set_scale(koefX, koefY);
        mImageHandle.set_linear_filter(true);
        mImageHandle.set_rotation_hotspot(origin_top_left, 0, 0);

        mCollision = CL_CollisionOutline(resourceManager().getImagePath(textureSection, textureName), accuracy_poor);
        //mCollision.set_translation(100, 100);
        mCollision.set_inside_test(true);
        mCollision.set_scale(koefX, koefY);
        mCollision.set_rotation_hotspot(origin_top_left, 0, 0);
        mCollision.optimize();
    }
}

void PhysicObject::updateVisual(float newX, float newY)
{
    if(!mImageHandle.is_null())
    {
        mImageHandle.set_angle(CL_Angle::from_radians(mBody->getBody()->GetAngle()));
        mImageHandle.draw(mGC, newX, newY);

        mCollision.draw(0, 0, CL_Colorf::yellow, mGC);
        mCollision.set_translation(newX, newY);
        //std::cout << mCollision.get_translation() << "\n";

    }
}

void PhysicObject::step(float32 elapsed)
{
}

void PhysicObject::setPosition(CL_Pointf newPos)
{
    // TODO: Implement me
    throw CL_Exception("Can't move body, after it created.");
}

CL_Pointf PhysicObject::getPosition()
{
    b2Vec2 position = mBody->getBody()->GetPosition();
    return CL_Pointf(Meters2Pixels(position.x), Meters2Pixels(position.y));
}

CL_Rectf PhysicObject::getRectangle()
{
    b2AABB rect;
    b2Fixture* fixture = mBody->getBody()->GetFixtureList();
    if (fixture != NULL) rect = fixture->GetAABB();
    for(; fixture != NULL; fixture=fixture->GetNext())
        rect.Combine(rect, fixture->GetAABB());

    return CL_Rectf(Meters2Pixels(rect.lowerBound.x), Meters2Pixels(rect.lowerBound.y),
            Meters2Pixels(rect.upperBound.x), Meters2Pixels(rect.upperBound.y));
}

void PhysicObject::update(float elapsed)
{
    step(elapsed);
}

boost::shared_ptr<Body> PhysicObject::getBody()
{
    return mBody;
}

std::vector< boost::shared_ptr<Action> > PhysicObject::getAvailableActions()
{
    return mActions;
}

boost::shared_ptr<Body> ParsePhysicBody(CL_DomElement body, std::string& desc);

boost::shared_ptr<Object> PhysicObject::ParsePhysicObject(CL_DomElement* tag, std::string& desc)
{
    PhysicObject* result = new PhysicObject(); // Smart pointer will be created at the end


    // Parsing visuals
    {
        CL_DomNodeList VisualTags = tag->get_elements_by_tag_name("Visual");
        if (VisualTags.get_length() == 1)
        {
            std::string textureName;
            std::string textureSection;
            float width = -1;
            float height = -1;

            CL_DomNodeList childList = VisualTags.item(0).get_child_nodes();
            for (int i=0; i < childList.get_length(); ++i)
            {
                CL_DomElement tag2 = childList.item(i).to_element();
                if (tag2.get_node_name() == "Size")
                {
                    width = boost::lexical_cast<float>(tag2.get_attribute("width").c_str());
                    height = boost::lexical_cast<float>(tag2.get_attribute("height").c_str());
                }
                if (tag2.get_node_name() == "Texture")
                {
                    textureSection = tag2.get_attribute("section").c_str();
                    textureName = tag2.get_attribute("name").c_str();
                }
            }

            result->setVisual(textureName, textureSection, width, height);
        }
        else if(VisualTags.get_length() > 1 )
        {
            throw CL_Exception("Error: only one tag Visual may be in Object.");
        }
    }
    // Parsing physic
    {
        CL_DomNodeList bodyTags = tag->get_elements_by_tag_name("Body");
        if (bodyTags.get_length() == 1)
        {
            CL_DomElement bodyTag = bodyTags.item(0).to_element();
            boost::shared_ptr<Body> physic = ParsePhysicBody(bodyTag, desc);
            result->mBody = physic;
            std::vector< boost::shared_ptr<Body> > bodies;
            bodies.push_back(physic);
            result->mActions.push_back(boost::shared_ptr<Action>(new TestAction()));
        }
    }

    return boost::shared_ptr<Object>(result);
}

bool PhysicObject::checkCollision(CL_Pointf point)
{
    return mCollision.point_inside(point);
}

