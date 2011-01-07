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

#include "Physic/Body.hpp"

#include "Physic/Impact.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/BodyVisual.hpp"
#include "World/Objects/Object.hpp"
#include "Physic/BodyPart.hpp"
#include "Physic/BodyMaterial.hpp"

Body::Body(b2Body* body)
{
    mBody = body;

    body->SetUserData(this);
    mShouldFreeB2Body = true;
    mParentWorld= mBody->GetWorld();
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

std::string Body::getName() { return mName; }
void Body::setName(const std::string &name) { mName = name; }
std::vector< boost::shared_ptr<BodyPart> > Body::getParts(){  return mParts; }

void Body::step(float32 elapsed)
{
    std::vector< boost::shared_ptr<BodyPart> >::iterator partsIt;
    for(partsIt = mParts.begin(); partsIt != mParts.end(); ++partsIt)
    {
        (*partsIt)->step(elapsed);
    }
}
