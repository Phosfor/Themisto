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

#include "World/Objects/Leaves.hpp"

Leaves::Leaves(uint16_t maxLeaves) : mFirstTime(true)
{
    srand(time(NULL));
    mGC = appManager().getGraphic();
    mMaxObjects = maxLeaves;

    functionValue = +1;
}

void Leaves::init()
{
    float windPower = worldManager().getWindPower();
    for (uint16_t i=0; i < mMaxObjects; ++i)
    {
        mLeaves.push_back(LeafData());
        processLeaves(mGC, windPower, mLeaves[i], true);
    }
}

void Leaves::processLeaves(CL_GraphicContext &gc, float windPower, LeafData &current, bool firstTime)
{
    current.timer = current.addedAngle = 0;
    current.x_speed = current.y_speed = 0;

    current.animation = false;

    // The height where new leaf will be located
    current.y = ScreenResolutionY - ScreenResolutionY*((rand()%6+2)/10.0);

    // Some magic numbers for the leaf trajectory
    current.k1 = rand() % 30 + 20;
    current.k2 = (rand() % 7 + 3) / 10.0;

    // Load some random leaf surface
    uint16_t size = resourceManager().sectionHandle("Leaves").get_child_nodes().get_length();
    current.leafType = rand() % size;

    current.imageHandle = resourceManager().getSprite("Leaves", boost::lexical_cast<std::string>(current.leafType));

    // Rotate leaf surface at some random angle
    uint16_t angle = rand() % 360 + 1;
    current.imageHandle.set_angle(CL_Angle::from_degrees(angle));

    float scale = static_cast<float>(rand()% 55 + 50) / 100.0f;
    current.imageHandle.set_scale(scale, scale);

    current.imageHandle.set_color(CL_Colorf(17/255.0f, 30/255.0f, 35/255.0f));

    current.speed_koef = rand()%70 + 50;

    // Where to create new leaf (refers to the wind blowing direction)
    if (firstTime)
    {
        current.x = rand() % ScreenResolutionX;
        current.timeout = 0;
    }
    else
    {
        if (windPower < 0)
            current.x = ScreenResolutionX;
        else
            current.x = 0 - current.imageHandle.get_width();

        current.timeout = rand() % 250;
    }
}

void Leaves::update(float elapsed)
{

}

void Leaves::updateVisual(float newX, float newY)
{
    float elapsed = appManager().getElapsed() / 1000.0f;
    float windPower = worldManager().getWindPower();
    float newSpeed1 = windPower * elapsed * 0.4;
    float newSpeed2 = Gravity * elapsed;
    for (uint16_t i=0; i < mMaxObjects; i++)
    {
        LeafData &current = mLeaves[i];
        if (current.timeout > 0)
        {
            current.timeout--;
        }
        else
        {
            if (windPower > 0) {
                if (current.x > ScreenResolutionX + 10 || current.y > ScreenResolutionY)
                    processLeaves(mGC, windPower, current);
            } else {
                if (current.x < -current.imageHandle.get_width() || current.y > ScreenResolutionY)
                    processLeaves(mGC, windPower, current);
            }

            current.x += current.x_speed * elapsed;
            current.y += current.y_speed * elapsed;

            float y = current.k1 * sinf(current.k2 * Deg2Rad(current.x)) + current.y;

            float tempValue = current.k1 * current.k2 * cosf(current.k2 * Deg2Rad(current.x));

            if (tempValue * functionValue < 0)
            {
                current.animation = true;
            }
            functionValue = tempValue;

            if (current.animation)
            {
                current.timer += elapsed * 1000.0;
                if (current.timer >= 100)
                {
                    uint16_t angle = rand() % 15 + 5;
                    current.addedAngle += angle;

                    current.imageHandle.rotate_pitch(CL_Angle::from_degrees(angle));

                    current.timer = 0;

                    if (current.addedAngle >= 180)
                    {
                        current.animation = false;
                        current.addedAngle = 0;
                    }
                }
            }

            current.x_speed = current.speed_koef * newSpeed1;
            current.y_speed = newSpeed2;
            current.imageHandle.draw(mGC, current.x, y);
        }
    }
}

boost::shared_ptr<Object> Leaves::ParseLeaves(CL_DomElement tag)
{
    float max = 0;
    if(tag.has_attribute("maxLeaves"))
    {
        std::string maxStr = tag.get_attribute("maxLeaves").c_str();
        max = boost::lexical_cast<float>(maxStr);
    }

    float area = Pixels2Meters(ScreenResolutionX) * Pixels2Meters(ScreenResolutionY);
    return boost::shared_ptr<Object>(new Leaves(max * area));
}

bool Leaves::checkCollision(CL_Pointf point) { return false; }

CL_Rectf Leaves::getRectangle()
{
    return CL_Rectf(0, 0, ScreenResolutionX, ScreenResolutionY);
}

CL_Pointf Leaves::getPosition()
{
    return CL_Pointf(0, 0);
}

void Leaves::setPosition(CL_Pointf newPos)
{
}
