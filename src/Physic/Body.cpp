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

#include "Physic/Body.hpp"

Body::Body()
{
    mName = worldManager().generateUniqueID();
    mShouldFreeB2Body = false;
}

Body::~Body()
{
    if(mShouldFreeB2Body && mBody) mBody->GetWorld()->DestroyBody(mBody);
}

std::string Body::getProperty(const std::string &name)
{
    std::map<std::string, std::string>::iterator findResult = mProperties.find(name);
    if(findResult != mProperties.end())
    {
        return (*findResult).second;
    }
    else
    {
        return "";
    }
}
void Body::setProperty(const std::string &name, const std::string &value)
{
    mProperties[name] = value;
    mPropertyChanged.invoke(name, value);
}

void Body::applyImpact(boost::shared_ptr<Impact> impact)
{
    mAppliedImpacts.push_front(impact);
    mImpactApplied.invoke(impact);
}

void Body::cancelImpact(boost::shared_ptr<Impact> impact)
{
    mAppliedImpacts.remove(impact);
    mImpactCanceled.invoke(impact);
}

std::string Body::getName()
{
     return mName;
}
void Body::setName(const std::string &value)
{
    mName.assign(value);
}

b2Body* Body::getBody()
{
    return mBody;
}
void Body::setBody(b2Body* body)
{
    mBody = body;
    body->SetUserData(this);
}
std::list< boost::shared_ptr<Impact> > Body::getAppliedImpacts()
{
    return mAppliedImpacts;
}
