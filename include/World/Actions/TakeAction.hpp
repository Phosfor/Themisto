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

#ifndef _TAKE_ACTION_HPP_
#define _TAKE_ACTION_HPP_

#include "World/Actions/Action.hpp"
#include "Core/ResourceManager.hpp"
#include "Physic/Body.hpp"
#include "Core/PhysicManager.hpp"
#include "World/Objects/Actor.hpp"



static const float TrunkRotateSpeed = 3.14/2; // To 90 degrees per second
class TakeAction: public Action
{
    private:
        enum TakeActionStates
        {
            Error = 0,
            NotActive,
            RotatingTrunk
        };

        Actor* mActor;
        std::vector< boost::shared_ptr<Body> > mBodies;
        std::map<b2Vec2, b2Body*> mTakePoints;
        std::pair<b2Vec2, b2Body*> mNearestTakePoint;
        b2PrismaticJoint* mHandJoint;
        TakeActionStates mState;
        float mCriticalAngle;

    public:
        explicit TakeAction(std::vector< boost::shared_ptr<Body> > bodies);


        b2PrismaticJoint* getHandJoint();

        // Action implementation
        std::string getName();
        std::string getDescription();
        CL_Sprite getIcon();
        std::vector<std::string> getTextureInfo();
        void beginExecution(Actor* actor);
        void stopExecution();
        bool canExecute(Actor* actor);
        bool executing();
        void update(float step);
        void updateVisual(float actorX, float actorY);
};

#endif
