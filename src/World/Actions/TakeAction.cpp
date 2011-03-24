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

#include "World/Actions/TakeAction.hpp"

bool operator< (const b2Vec2& one, const b2Vec2& two)
{
    return one.x + one.y < two.x + two.y;
};

TakeAction::TakeAction(std::vector< boost::shared_ptr<Body> > bodies)
{
    mState = NotActive;
    mBodies = bodies;
    mHandJoint = NULL;
    mActor = NULL;
    // Read take points
    for(unsigned int b = 0; b<bodies.size(); ++b)
    {
        boost::shared_ptr<Body> body = bodies[b];
        std::string points = body->getProperty("TakePoints");
        std::cout << points << std::endl;
        if(points != "")
        {
            std::vector<std::string> splitResult;
            boost::split(splitResult, points, boost::is_any_of("()"));
            std::cout << "splitResult.size() " << splitResult.size() << std::endl;
            for(unsigned int s = 0; s < splitResult.size(); ++s)
            {
                std::cout << splitResult[s] << "?" << std::endl;
                boost::trim(splitResult[s]);
                if(splitResult[s] != "")
                {
                    std::vector<std::string> xy;
                    boost::split(xy, splitResult[s], boost::is_any_of(","));
                    std::cout << xy.size() << " xy.size()" << std::endl;
                    if(xy.size() == 2)
                    {
                        std::cout << xy[0] << ',' << xy[1] << std::endl;
                        b2Vec2 point(boost::lexical_cast<float>(xy[0]), boost::lexical_cast<float>(xy[1]));
                        point.x = Pixels2Meters(point.x);
                        point.y = Pixels2Meters(point.y);
                        std::pair<b2Vec2, b2Body*> item(point, body->getBody());
                        mTakePoints.insert(item);
                    }
                    else
                    {
                        throw CL_Exception("Invalid TakePoints attribute format.");
                    }
                }
            }
        }
    }
}

bool TakeAction::executing()
{
    return mState == RotatingTrunk;
}

b2PrismaticJoint* TakeAction::getHandJoint()
{
    return mHandJoint;
}

std::string TakeAction::getName()
{
    return "Взять";
}
std::string TakeAction::getDescription()
{
    return "Взять предмет в руки";
}
CL_Sprite TakeAction::getIcon()
{
    return resourceManager().getSprite("Actions", "take");
}
std::vector<std::string> TakeAction::getTextureInfo()
{
    std::vector<std::string> res;
    res.push_back("Actions");
    res.push_back("take");
    return res;
}
std::pair<b2Vec2, b2Body*> _getNearestPoint(std::map<b2Vec2, b2Body*> points, b2Vec2 anchor, float* dist)
{
    std::cout << "points.size() "<<  points.size() << std::endl;
    if(points.size() > 0)
    {
        std::cout << "anchor: " << anchor.x << "," << anchor.y << std::endl;
        std::pair<b2Vec2, b2Body*> result = *(points.begin());
        std::cout << "first point: " << result.first.x << ',' << result.first.y << std::endl;
        b2Vec2 wFirstPoint = result.second->GetWorldPoint(result.first);
        std::cout << "wfirst point: " << wFirstPoint.x << ',' << wFirstPoint.y << std::endl;
        float minDistance = b2Distance(wFirstPoint, anchor);
        std::map<b2Vec2, b2Body*>::iterator it;
        for(it = ++(points.begin()); it != points.end(); ++it)
        {
            std::cout << "next point: " << it->first.x << ',' << it->first.y << std::endl;
            b2Vec2 wPoint = it->second->GetWorldPoint(it->first);
            std::cout << "wnext point: " << wPoint.x << ',' << wPoint.y << std::endl;
            float distance = b2Distance(wPoint, anchor);
            if(distance < minDistance)
            {
                minDistance = distance;
                result = *it;
            }
        }
        *dist = minDistance;
        return result;
    }
    else
    {
        // Its impossible I suppose
        return std::pair<b2Vec2, b2Body*>(b2Vec2(0,0), NULL);
    }
}

b2PrismaticJoint* _createJoint(b2Body* shoulder, b2Body* body, b2Vec2 _anchorShoulder, b2Vec2 anchorBody, float force)
{
    b2PrismaticJointDef jointDef;
    jointDef.Initialize(shoulder, body, _anchorShoulder, _anchorShoulder - anchorBody);
    jointDef.maxMotorForce = force;
    jointDef.motorSpeed = 0.5;
    jointDef.enableMotor = true;
    jointDef.lowerTranslation = 0;
    jointDef.upperTranslation = b2Distance(_anchorShoulder, anchorBody);
    jointDef.enableLimit = true;
    return (b2PrismaticJoint*) physicManager().getWorld().CreateJoint(&jointDef);
}

float _calculateCriticalAngle(b2RevoluteJoint* trunkJoint, b2Vec2 takePoint)
{
    b2Vec2 criticalVector = trunkJoint->GetAnchorA(); // AnchorA == AnchorB
    criticalVector.Normalize();
    return acos(criticalVector.x) + Pi/2;
}

void TakeAction::beginExecution(Actor* actor)
{

    if(canExecute(actor))
    {
        std::cout << "TakeAction is to begin executing." << std::endl;
        mExecutionBegun.invoke(this);
        mActor = actor;
        b2Vec2 shoulderPoint = actor->getShoulder()->getBody()->GetWorldCenter();
        float distance = 0;
        mNearestTakePoint = _getNearestPoint(mTakePoints, shoulderPoint, &distance);
        float handLength = actor->getHandLength();
        // Make distance between shoulder and takePoint nearest to
        // handLength value by rotating trunk of actor (rotate only to minimize distance, not enlarge)
        if( distance  > handLength)
        {
            mState = RotatingTrunk;
            b2RevoluteJoint* trunkJoint = actor->getTrunkJoint();

            mCriticalAngle = _calculateCriticalAngle(trunkJoint, mNearestTakePoint.first);
            float speed = 0;
            if(mCriticalAngle > Pi/2)
            {
                speed = -TrunkRotateSpeed;
                trunkJoint->SetLimits(-mCriticalAngle, 0);
            }
            else
            {
                speed = TrunkRotateSpeed;
                trunkJoint->SetLimits(0, mCriticalAngle);
            }
            trunkJoint->SetMaxMotorTorque(1);
            trunkJoint->EnableMotor(true);
        }
        else
        {
            mHandJoint = _createJoint(actor->getShoulder()->getBody(), mNearestTakePoint.second,
                        shoulderPoint, mNearestTakePoint.first, mActor->getHandPower());
            mExecutionCompleted.invoke(this);
        }
    }
    else
    {
        std::cout << "Can't execute TakeAction: object too far." << std::endl;
        // Caller, you are stupid, call canExecute first
    }
}
void TakeAction::stopExecution()
{
    if(mState == RotatingTrunk)
    {
        mState = NotActive;
        mActor->goToNormalState();
        mExecutionStopped.invoke(this);
    }
}
bool TakeAction::canExecute(Actor* actor)
{
    b2Vec2 shoulderPoint = actor->getShoulder()->getBody()->GetWorldCenter();
    float distance = 0;
    std::pair<b2Vec2, b2Body*> nearestTakePoint = _getNearestPoint(mTakePoints, shoulderPoint, &distance);

    float trunkLength = b2Distance(shoulderPoint, actor->getTrunkJoint()->GetAnchorA());
    //float distance = b2Distance(nearestTakePoint.second->GetWorldPoint(nearestTakePoint.first), shoulderPoint);
    std::cout << "Distance: " << distance << std::endl;
    std::cout << "trunk: " << trunkLength << " hand: "<< actor->getHandLength() << std::endl;
    return  distance <= (actor->getHandLength() + trunkLength);
    return false;
}
void TakeAction::update(float step)
{
    if(mState == RotatingTrunk)
    {
        b2Vec2 shoulderPoint = mActor->getShoulder()->getBody()->GetWorldCenter();
        if(b2Distance(shoulderPoint, mNearestTakePoint.first) <= mActor->getHandLength())
        {
            mHandJoint = _createJoint(mActor->getShoulder()->getBody(), mNearestTakePoint.second,
                        shoulderPoint, mNearestTakePoint.first, mActor->getHandPower());
            mActor->goToNormalState();
            mState = NotActive;
            mExecutionCompleted.invoke(this);
        }
    }
}
void TakeAction::updateVisual(float actorX, float actorY)
{
}
