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

#include "World/Objects/LightColumn.hpp"

LightColumn::LightColumn()
{
    mName = worldManager().generateUniqueID();
    mGC = appManager().getGraphic();

    mAbsTime = 0.0f;
}

void LightColumn::init() {}

void LightColumn::setVisual(std::string textureName, std::string textureSection)
{
    setVisual(textureName, textureSection, -1, -1);
}

void LightColumn::setVisual(std::string textureName, std::string textureSection, float width, float height)
{
    mImageHandle = resourceManager().getSprite(textureSection, textureName);
    if(!mImageHandle.is_null())
    {
        float koefX, koefY;
        koefX = koefY = 1;
        if(width > 0) koefX = width / mImageHandle.get_width();
        if(height > 0) koefY = height / mImageHandle.get_height();
        mImageHandle.set_scale(koefX, koefY);
        mImageHandle.set_linear_filter(true);
        mImageHandle.set_rotation_hotspot(origin_top_left, 0, 0);
        mBoundingBox = CL_Rectf(mPos.x, mPos.y, mPos.x + mImageHandle.get_width() * koefX,
                mPos.y + mImageHandle.get_height() * koefY);

        mCollision = CL_CollisionOutline(resourceManager().getImagePath(textureSection, textureName), accuracy_poor);
        //mCollision.set_translation(100, 100);
        mCollision.set_inside_test(true);
        mCollision.set_scale(koefX, koefY);
        mCollision.set_rotation_hotspot(origin_top_left, 0, 0);
        mCollision.optimize();
    }
}

void LightColumn::addLighting(const std::string &textureName, const std::string &textureSection, float width, float height)
{
    CL_Sprite light = resourceManager().getSprite(textureSection, textureName);
    //light.set_alignment(origin_center);
    mLightings.push_back(light);
    float koefX, koefY;
    koefX = koefY = 1;
    if(width > 0) koefX = width / mLightings[mLightings.size()-1].get_width();
    if(height > 0) koefY = height / mLightings[mLightings.size()-1].get_height();
    mLightings[mLightings.size()-1].set_scale(koefX, koefY);
    mLightings[mLightings.size()-1].set_linear_filter(true);

    CL_Pointf pos = mLightingsPos[mLightingsPos.size()-1];
    mBoundingBox.bounding_rect(CL_Rectf(mPos.x + pos.x,
                mPos.y + pos.y,
                mPos.x + pos.x + light.get_width(),
                mPos.y + pos.y + light.get_height()));
}

void LightColumn::addBug(LightBug &bug)
{
    mBugs.push_back(bug);
}

void LightColumn::setLightingPos(CL_Pointf pos)
{
    mLightingsPos.push_back(pos);
}

LightColumn::~LightColumn()
{
}

void LightColumn::updateVisual(float newX, float newY)
{
    if(!mImageHandle.is_null())
    {
        // Drawing postlight
        mImageHandle.draw(mGC, newX, newY);
        mCollision.draw(0, 0, CL_Colorf::yellow, mGC);
        mCollision.set_translation(newX, newY);
        //std::cout << mCollision.get_translation() << "\n";

        // Drawing lightings
        for (uint16_t i=0; i < mLightings.size(); ++i)
            mLightings[i].draw(mGC, newX + mLightingsPos[i].x, newY + mLightingsPos[i].y);

        BOOST_FOREACH(LightBug &bug, mBugs)
        {
            bug.imageHandle.draw(mGC, newX + bug.x + bug.offsetX, newY + bug.y + bug.offsetY);
        }
    }
}

CL_Pointf LightColumn::getPosition()
{
    return mPos;
}

void LightColumn::setPosition(CL_Pointf newPos)
{
    mPos = newPos;
}

CL_Rectf LightColumn::getRectangle()
{
    return mBoundingBox;
}

void LightColumn::update(float elapsed)
{
    mAbsTime += elapsed / 3.5f;

    BOOST_FOREACH(LightBug &bug, mBugs)
    {
        bug.x = bug.boundingValue * sinf(bug.koef1 * mAbsTime + bug.theta);
        bug.y = bug.boundingValue * sinf(bug.koef2 * mAbsTime);
    }
}

boost::shared_ptr<Object> LightColumn::ParseLightColumn(CL_DomElement* tag, std::string& desc)
{
    LightColumn* result = new LightColumn(); // Smart pointer will be created at the end

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
                if (tag2.get_node_name() == "Position")
                {
                    float x = boost::lexical_cast<float>(tag2.get_attribute("x").c_str());
                    float y = boost::lexical_cast<float>(tag2.get_attribute("y").c_str());

                    result->setPosition(CL_Pointf(x, y));
                }
            }

            result->setVisual(textureName, textureSection, width, height);
        }
        else if(VisualTags.get_length() > 1 )
        {
            throw CL_Exception("Error: only one tag Visual may be in Object.");
        }

        // Parsing lightings images
        CL_DomNodeList lightingNodes = tag->get_elements_by_tag_name("Lighting");
        for (int i=0; i < lightingNodes.get_length(); ++i)
        {
            CL_DomElement lightHandle = lightingNodes.item(i).to_element();
            CL_DomNodeList lightChildren = lightHandle.get_child_nodes();

            int width, height;
            width = height = 1;
            std::string textureSection, textureName;
            float xPos, yPos;
            xPos = yPos = 0;
            for (int j=0; j < lightChildren.get_length(); ++j)
            {
                CL_DomElement child = lightChildren.item(j).to_element();

                if (child.get_node_name() == "Size")
                {
                    width = boost::lexical_cast<int>(child.get_attribute("width").c_str());
                    height = boost::lexical_cast<int>(child.get_attribute("height").c_str());

                }
                if (child.get_node_name() == "Texture")
                {
                    textureSection = child.get_attribute("section").c_str();
                    textureName = child.get_attribute("name").c_str();
                }
                if (child.get_node_name() == "Position")
                {
                    xPos = boost::lexical_cast<float>(child.get_attribute("x").c_str());
                    yPos = boost::lexical_cast<float>(child.get_attribute("y").c_str());
                }
            }
            result->setLightingPos(CL_Pointf(xPos, yPos));
            result->addLighting(textureName, textureSection, width, height);
        }

        // Parsing bugs images
        CL_DomNodeList bugNodes = tag->get_elements_by_tag_name("Bug");
        for (int i=0; i < bugNodes.get_length(); ++i)
        {
            CL_DomElement bugHandle = bugNodes.item(i).to_element();
            CL_DomNodeList bugChildren = bugHandle.get_child_nodes();

            std::string textureSection, textureName;
            float bounding = 0;
            float a, b;
            a = b = 1;
            float xPos, yPos;
            xPos = yPos = 0;
            float scaleX, scaleY;
            scaleX = scaleY = 1;
            for (int j=0; j < bugChildren.get_length(); ++j)
            {
                CL_DomElement child = bugChildren.item(j).to_element();

                if (child.get_node_name() == "Bounding")
                {
                    bounding = boost::lexical_cast<int>(child.get_attribute("value").c_str());

                }
                if (child.get_node_name() == "Texture")
                {
                    textureSection = child.get_attribute("section").c_str();
                    textureName = child.get_attribute("name").c_str();
                }
                if (child.get_node_name() == "Koef")
                {
                    a = boost::lexical_cast<float>(child.get_attribute("a").c_str());
                    b = boost::lexical_cast<float>(child.get_attribute("b").c_str());
                }
                if (child.get_node_name() == "Position")
                {
                    xPos = boost::lexical_cast<float>(child.get_attribute("x").c_str());
                    yPos = boost::lexical_cast<float>(child.get_attribute("y").c_str());
                }
                if (child.get_node_name() == "TextureScale")
                {
                    scaleX = boost::lexical_cast<float>(child.get_attribute("x").c_str());
                    scaleY = boost::lexical_cast<float>(child.get_attribute("y").c_str());
                }
            }
            LightBug bug;

            CL_Sprite light = resourceManager().getSprite(textureSection, textureName);
            light.set_alignment(origin_center);
            light.set_scale(scaleX, scaleY);

            bug.boundingValue = bounding;
            bug.koef1 = a;
            bug.koef2 = b;
            bug.offsetX = xPos;
            bug.offsetY = yPos;
            bug.imageHandle = light;

            result->addBug(bug);
        }
    }

    return boost::shared_ptr<Object>(result);
}

bool LightColumn::checkCollision(CL_Pointf point)
{
    return mCollision.point_inside(point);
}
