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


#include "Physic/BodyPart.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/Impact.hpp"
#include "Physic/BodyMaterial.hpp"


LightColumn::LightColumn()
{
    mName = worldManager().generateUniqueID();
    mType = LightColumnObject;
    mGC = appManager().getGraphic();

    // bugs
    mAbsTime = x = y = x_ = y_ = x__ = y__ = 0.0f;
}

void LightColumn::setVisual(std::string textureName, std::string textureSection)
{
    setVisual(textureName, textureSection, -1, -1);
}

void LightColumn::setVisual(std::string textureName, std::string textureSection, float width, float height)
{
    mImageHandle = resourceManager().getSprite(textureSection, textureName);
    // Improove this shit
    mLighting = resourceManager().getSprite("Stuff", "light");
    mBug = resourceManager().getSprite("Stars", "3");
    mBug.set_scale(0.7, 0.7);
    //mLighting.set_scale(3, 3);
    mLighting.set_alignment(origin_center);
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
        mBoundingBox = CL_Rectf(mPos.x, mPos.y, mPos.x + mImageHandle.get_width() * koefX, 
                mPos.y + mImageHandle.get_height() * koefY);
    }
}

LightColumn::~LightColumn()
{
}

void LightColumn::updateVisual(float newX, float newY)
{
    if(!mImageHandle.is_null())
    {
        /*float centerX = newX + mImageHandle.get_width()/3.0f;
        float centerY = newY + mImageHandle.get_width()/3.0f;*/
        // x;y
        // x_;y_
        float len = sqrt( x*x + y*y );
        float len_ = sqrt( x_*x_ + y_*y_ );
        float len__ = sqrt( x__*x__ + y__*y__ );

        float alpha = 1 - len / mImageHandle.get_width()*1.5f;
        float alpha_ = 1 - len_ / mImageHandle.get_width()*1.5f;
        float alpha__ = 1 - len__ / mImageHandle.get_width()*1.5f;

        CL_Colorf col;
        mImageHandle.draw(mGC, newX, newY);
        mLighting.draw(mGC, newX + mImageHandle.get_width()/3.0f, newY + mImageHandle.get_width()/3.0f);

        col.set_alpha(alpha);
        /*CL_Draw::circle(mGC, newX + mImageHandle.get_width()/3.0f + x, newY + mImageHandle.get_width()/3.0f + y,
                1.0f, col);*/
        mBug.draw(mGC, newX + mImageHandle.get_width()/3.0f + x, newY + mImageHandle.get_width()/3.0f + y);
        mBug.set_alpha(alpha);

        mBug.draw(mGC, newX + mImageHandle.get_width()/3.0f + x_, newY + mImageHandle.get_width()/3.0f + y_);
        mBug.set_alpha(alpha_);

        mBug.draw(mGC, newX + mImageHandle.get_width()/3.0f + x__, newY + mImageHandle.get_width()/3.0f + y__);
        mBug.set_alpha(alpha__);
        /*col.set_alpha(alpha_);
        CL_Draw::circle(mGC, x_ + newX + mImageHandle.get_width()/3.0f, y_ + newY + mImageHandle.get_width()/3.0f,
                1.0f, col);*/
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

    x = 70 * sinf(11*mAbsTime);
    y = 70 * sinf(13*mAbsTime);

    x_ = 70 * sinf(15*mAbsTime + 1.5f);
    y_ = 70 * sinf(23*mAbsTime);

    x__ = 70 * sinf(17*mAbsTime + 1.2f);
    y__ = 70 * sinf(27*mAbsTime);
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
    }

    return boost::shared_ptr<Object>(result);
}

