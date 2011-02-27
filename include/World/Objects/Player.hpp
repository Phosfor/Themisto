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

#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_


#include <Box2D/Box2D.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <boost/lexical_cast.hpp>
#include <ClanLib/core.h>

#include "World/WorldManager.hpp"
#include "World/Objects/Object.hpp"
#include "Core/Utils.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/PhysicManager.hpp"
#include "Core/InputManager.hpp"
#include "World/Objects/Actor.hpp"

class Body;
class Impact;

class Player: public Actor
{
    private:
        InputManager *mInputManager;

        // State
        bool mGoingLeft;
        bool mGoingRight;
        bool mJumpBegun;

        // Bodies
        boost::shared_ptr<Body> mTopBoxBody;
        boost::shared_ptr<Body> mRollBaseBody;
        boost::shared_ptr<Body> mMiddleBoxBody;
        boost::shared_ptr<Body> mShoulderBody;
        std::vector< boost::shared_ptr<Body> > mRolls;
        std::vector< boost::shared_ptr<Body> > mBodies;

        // Joints
        std::vector<b2RevoluteJoint*> mRollJoints;
        b2RevoluteJoint* mShoulderJoint;
        b2RevoluteJoint* mTopBoxJoint;
        b2PrismaticJoint* mJumpJoint;

        // Slots
        CL_Slot mKeyDownSlot;
        CL_Slot mKeyUpSlot;

        // Params
        float mRollAngularVelocity;
        float mJumpVelocity;
        float mJumpDeep;

        // Visual
        CL_GraphicContext mGC;

        void keyDown(const CL_InputEvent& ev, const CL_InputState& state);
        void keyUp(const CL_InputEvent& ev, const CL_InputState& state);

    public:
        static const float PlayerHeight = 1.7;
        static const float PlayerWidth = 0.5;
        static const float PlayerMass = 70.0;
        static const float PlayerSpeed = 2.0;

        explicit Player();

        void updateVisual();
        void step(float32 elapsed); // Physic

        // Parsing object
        static boost::shared_ptr<Object> ParsePlayer(CL_DomElement* node, std::string &desc);

        // --- Object implementation ---
        CL_Pointf getPosition();
        CL_Rectf getRectangle();
        void setPosition(CL_Pointf newPos);

        void init();
        void update(float elapsed);
        void updateVisual(float newX, float newY);

        // --- Actor implementation ---

         void goLeft();
         void goRight();
         void stopMoving();
         void beginJump();
         void endJump();
         boost::shared_ptr<Body> getShoulder();
         b2RevoluteJoint* getShoulderJoint();
         boost::shared_ptr<Body> getTrunk();
         b2RevoluteJoint* getTrunkJoint();
         float getHandPower();
         float getHandLength();
         void goToNormalState();
};

#endif
