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




class Body;
class Impact;


class Player: public Object
{
    private:
        InputManager *mInputManager;
        boost::shared_ptr<Body> mTopBoxBody;
        boost::shared_ptr<Body> mRollBaseBody;
        boost::shared_ptr<Body> mCircleBody1;
        boost::shared_ptr<Body> mCircleBody2;
        boost::shared_ptr<Body> mCircleBody3;
        boost::shared_ptr<Body> mCircleBody4;
        CL_Slot mKeyDownSlot;
        CL_Slot mKeyUpSlot;
        float mRollAngularVelocity;
        float mJumpVelocity;

        // Visual
        CL_GraphicContext mGC;

        void keyDown(const CL_InputEvent& ev, const CL_InputState& state);
        void keyUp(const CL_InputEvent& ev, const CL_InputState& state);

    public:
        static const float PlayerHeight = 4;
        static const float PlayerWidth = 1.5;
        static const float PlayerMass = 30.0;
        static const float PlayerSpeed = 5.0;

        explicit Player();

        // Get/set
        void setPhysic(boost::shared_ptr<Body> bodyTopBox,
                       boost::shared_ptr<Body> bodyRollBase,
                       boost::shared_ptr<Body> bodyCircle1,
                       boost::shared_ptr<Body> bodyCircle2,
                       boost::shared_ptr<Body> bodyCircle3,
                       boost::shared_ptr<Body> bodyCircle4);


        void updateVisual();
        void step(float32 elapsed); // Physic

        // Parsing object
        static boost::shared_ptr<Object> ParsePlayer(CL_DomElement* node, std::string &desc);

        // --- Object implementation ---
        CL_Pointf getPosition();
        void setPosition(CL_Pointf newPos);
        void update(float elapsed);
        void updateVisual(float newX, float newY);
        CL_Rectf getRectangle();


};

#endif
