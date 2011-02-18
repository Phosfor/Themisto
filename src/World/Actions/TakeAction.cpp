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
    for(unsigned int i = 0; i<bodies.size(); ++i)
    {
        boost::shared_ptr<Body> body = bodies[i];
        std::string points = body->getProperty("TakePoints");
        if(points != "")
        {
            std::vector<std::string> splitResult;
            boost::split(splitResult, points, boost::is_any_of("()"));
            for(unsigned int s = 0; s < splitResult.size(); ++s)
            {
                std::vector<std::string> xy;
                boost::split(xy, splitResult[i], boost::is_any_of(","));
                b2Vec2 point(boost::lexical_cast<float>(xy[0]), boost::lexical_cast<float>(xy[1]));
                point = body->getBody()->GetWorldPoint(point);
                std::pair<b2Vec2, b2Body*> item(point, body->getBody());
                std::map<b2Vec2, b2Body*> mTakePoints2;
                mTakePoints2.insert(std::make_pair(point, body->getBody()));
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

std::pair<b2Vec2, b2Body*> _getNearestPoint(std::map<b2Vec2, b2Body*> points, b2Vec2 anchor)
{
    std::map<b2Vec2, b2Body*>::iterator it;
    if(points.size() > 0)
    {
        float minDistance = b2Distance(points.begin()->first, anchor);
        std::pair<b2Vec2, b2Body*> result = *(points.begin());
        for(it = ++(points.begin()); it != points.end(); ++it)
        {
            float distance = b2Distance(it->first, anchor);
            if(distance < minDistance)
            {
                minDistance = distance;
                result = *it;
            }
        }
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
        mExecutionBegun.invoke(this);
        mActor = actor;
        b2Vec2 shoulderPoint = actor->getShoulder()->getBody()->GetWorldCenter();
        mNearestTakePoint = _getNearestPoint(mTakePoints, shoulderPoint);
        float handLength = actor->getHandLength();
        // Make distance between shoulder and takePoint nearest to
        // handLength value by rotating trunk of actor (rotate only to minimize distance, not enlarge)
        if( b2Distance(mNearestTakePoint.first, shoulderPoint) > handLength)
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
    std::pair<b2Vec2, b2Body*> nearestTakePoint = _getNearestPoint(mTakePoints, shoulderPoint);
    float trunkLength = b2Distance(shoulderPoint, actor->getTrunkJoint()->GetAnchorA());
    return b2Distance(nearestTakePoint.first, shoulderPoint) <= (actor->getHandLength() + trunkLength);
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
