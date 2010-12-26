/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Physic/BodyVisual.hpp"

BodyVisual::BodyVisual()
{
}

void BodyVisual::redrawBody(Body &body)
{
    b2Vec2 position = body.getb2Body()->GetPosition();

    mImageHandle.rotate(CL_Angle::from_degrees(body.getb2Body()->GetAngle()));
    mImageHandle.draw(appManager().getGraphic(), Meters2Pixels(position.x), Meters2Pixels(position.y));
}

void BodyVisual::setBodyState(BodyState *state)
{
    mBodyState = state;
}

void BodyVisual::configureVisual()
{
    mImageHandle = resourceManager().getSprite(mSectionName, mTextureName);
    mImageHandle.set_alignment(origin_center);

    float koefX = mSizeWidth / mImageHandle.get_width();
    float koefY = mSizeHeight / mImageHandle.get_height();

    mImageHandle.set_scale(koefX, koefY);
}

bool BodyVisual::getVisualState()
{
    return mHasVisual;
}

void BodyVisual::setVisualState(bool state)
{
    mHasVisual = state;
}
