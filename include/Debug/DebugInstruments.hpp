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

#ifndef _DEGUG_INSTRUMENTS_HPP
#define _DEGUG_INSTRUMENTS_HPP

#include <Box2D/Dynamics/b2World.h>
#include "Physic/Body.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/PhysicManager.hpp"
#include "Core/InputManager.hpp"
#include <iostream>

class DebugDragAndDrop: public b2QueryCallback
{
    private:
        b2MouseJoint* mMouseJoint;
        b2Vec2 *mMousePos;
        b2Body* mAttachBody;

        void mouseDown(const CL_InputEvent &key, const CL_InputState &state);
        void mouseUp(const CL_InputEvent &key, const CL_InputState &state);
        void mouseMove(const CL_InputEvent &key, const CL_InputState &state);

        bool ReportFixture (b2Fixture *fixture);

        // Slots handles
        CL_Slot mDownSlot, mUpSlot, mMoveSlot;

    public:
        void init();
        ~DebugDragAndDrop();
};

#endif
