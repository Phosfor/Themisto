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

#include "Physic/BodyVisual.hpp"

BodyVisual::BodyVisual()
{
}

void BodyVisual::redrawBody()
{
    b2Body *body = mParentBody->getb2Body();
    mImageHandle.set_angle(CL_Angle::from_radians(body->GetAngle()));
    b2Vec2 position = body->GetPosition();
    mImageHandle.draw(appManager().getGraphic(), Meters2Pixels(position.x)+mXPos, Meters2Pixels(position.y)+mYPos);
}

void BodyVisual::configureVisual(boost::shared_ptr<Body> parent)
{
    mParentBody = parent;

    mImageHandle = resourceManager().getSprite(mSectionName, mTextureName);
    //mImageHandle.set_alignment(origin_);

    float koefX = mSizeWidth / mImageHandle.get_width();
    float koefY = mSizeHeight / mImageHandle.get_height();

    mImageHandle.set_scale(koefX, koefY);
    mImageHandle.set_linear_filter(true);
}

bool BodyVisual::getVisualState()
{
    return mHasVisual;
}

void BodyVisual::setVisualState(bool state)
{
    mHasVisual = state;
}
