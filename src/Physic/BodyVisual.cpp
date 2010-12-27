/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Physic/BodyVisual.hpp"

BodyVisual::BodyVisual()
{
}

void BodyVisual::redrawBody()
{
    b2Vec2 position = mParentBody->getb2Body()->GetPosition();

    mImageHandle.rotate(CL_Angle::from_degrees(mParentBody->getb2Body()->GetAngle()));
    mImageHandle.draw(appManager().getGraphic(), Meters2Pixels(position.x), Meters2Pixels(position.y));
}

void BodyVisual::setBodyState(BodyState *state)
{
    mBodyState = state;
}

void BodyVisual::configureVisual(Body *parent)
{
    mParentBody = parent;

    mImageHandle = resourceManager().getSprite(mSectionName, mTextureName);
    mImageHandle.set_alignment(origin_center);

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
