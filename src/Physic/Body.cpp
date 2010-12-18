/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Physic/Body.hpp"

Body::Body(b2Body* body)
{
    mBody = body;

    body->SetUserData(this);
    mShouldFreeB2Body = true;
    mParentWorld= mBody->GetWorld();
    mLastLocation = calculateLocation();
    world = &worldManager();
    mName = worldManager().generateUniqueID();
}

Body::~Body()
{
    if(mShouldFreeB2Body) mParentWorld->DestroyBody(mBody);
}

b2Body* Body::getb2Body()
{
    return mBody;
}

void Body::step(float32 elapsed)
{
    for(b2Fixture* fixture = mBody->GetFixtureList();
            fixture != NULL; fixture = fixture->GetNext())
    {
        BodyPart* part = (BodyPart*)fixture->GetUserData();
        if( part != NULL)
        {
            part->step(elapsed);
        }
    }

    b2AABB currentLocation = calculateLocation();

    b2Vec2 ll = mLastLocation.lowerBound;
    b2Vec2 cl = currentLocation.lowerBound;
    float deltaX = cl.x - ll.x;
    float deltaY = cl.y - ll.y;
    float cellSize = areaManager().getCellSize();
    // If body moved father then cell size
    if( abs(deltaX) > cellSize ||  abs(deltaY) > cellSize)
    {
        for(b2Fixture* fixture = mBody->GetFixtureList();
            fixture != NULL; fixture = fixture->GetNext())
        {
            b2AABB lastLocation = fixture->GetAABB();
            b2Vec2 delta(deltaX, deltaY);
            lastLocation.lowerBound = lastLocation.lowerBound - delta;
            lastLocation.upperBound = lastLocation.upperBound - delta;
            areaManager().reportNewFixtureLocation(&lastLocation, 
                    &fixture->GetAABB(), fixture);
        }
        mLastLocation = currentLocation;
    }
}

b2AABB Body::calculateLocation()
{
    b2AABB region;
    b2Fixture* firstFixture = mBody->GetFixtureList();
    if(firstFixture != NULL)
    {
        b2AABB r = firstFixture->GetAABB();
        region.lowerBound = r.lowerBound;
        region.upperBound = r.upperBound;

        for(b2Fixture* fixture = firstFixture->GetNext();
                fixture != NULL; fixture = fixture->GetNext())
        {
            region.Combine(region, fixture->GetAABB());
        }
    }
    else
    {
        region.lowerBound.Set(0,0);
        region.upperBound.Set(0,0);
    }
    return region;
}
