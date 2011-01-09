/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

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


#include "Physic/BodyPart.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/Impact.hpp"
#include "Physic/BodyMaterial.hpp"


PhysicObject::PhysicObject()
{
    mBody = NULL;
    mName = worldManager().generateUniqueID();
    mType = PhysicBodyObject;
    mShouldFreeB2Body = true;
    mGC = appManager().getGraphic();
}

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
    }
}

PhysicObject::~PhysicObject()
{
    if(mShouldFreeB2Body && mBody) mBody->GetWorld()->DestroyBody(mBody);
}

void PhysicObject::updateVisual(float newX, float newY)
{
    if(!mImageHandle.is_null())
    {
        mImageHandle.set_angle(CL_Angle::from_radians(mBody->GetAngle()));
        mImageHandle.draw(mGC, newX, newY);
    }
}

void PhysicObject::step(float32 elapsed)
{
    std::vector< boost::shared_ptr<BodyPart> >::iterator partsIt;
    for(partsIt = mParts.begin(); partsIt != mParts.end(); ++partsIt)
    {
        (*partsIt)->step(elapsed);
    }
}

CL_Pointf PhysicObject::getPosition()
{
    b2Vec2 position = mBody->GetPosition();
    return CL_Pointf(Meters2Pixels(position.x), Meters2Pixels(position.y));
}

CL_Rectf PhysicObject::getRectangle()
{
    b2AABB rect;
    b2Fixture* fixture = mBody->GetFixtureList();
    if (fixture != NULL) rect = fixture->GetAABB();
    for(; fixture != NULL; fixture=fixture->GetNext())
        rect.Combine(rect, fixture->GetAABB());

    return CL_Rectf(Meters2Pixels(rect.lowerBound.x), Meters2Pixels(rect.upperBound.y),
            Meters2Pixels(rect.upperBound.x), Meters2Pixels(rect.lowerBound.y));
}

void PhysicObject::update(float elapsed)
{
    step(elapsed);
}

b2Body* PhysicObject::getb2Body()
{
    return mBody;
}
void PhysicObject::setParts(std::vector< boost::shared_ptr<BodyPart> > parts)
{
    mParts = parts;
}

void PhysicObject::setb2Body(b2Body* body)
{
    if(mBody != NULL)
    {
        mBody->SetUserData(NULL);
    }
    mBody = body;
    mBody->SetUserData(this);
}

std::vector< boost::shared_ptr<BodyPart> >
PhysicObject::getParts(){  return mParts; }

std::pair< b2Body*, std::vector< boost::shared_ptr<BodyPart> > > ParsePhysicBody(CL_DomElement body, std::string& desc);

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
            std::pair<b2Body*, std::vector< boost::shared_ptr<BodyPart> > > physic = ParsePhysicBody(bodyTag, desc);
            result->setb2Body(physic.first);
            result->setParts(physic.second);
        }
    }

    return boost::shared_ptr<Object>(result);
}

