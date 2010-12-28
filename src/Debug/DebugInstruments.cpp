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

#include "Debug/DebugInstruments.hpp"

void DebugDragAndDrop::init()
{
    mMousePos = new b2Vec2;
    CL_InputDevice device = inputManager().getMouse();
    mDownSlot = device.sig_key_down().connect(this, &DebugDragAndDrop::mouseDown);
    mMoveSlot = device.sig_pointer_move().connect(this, &DebugDragAndDrop::mouseMove);
    mUpSlot = device.sig_key_up().connect(this, &DebugDragAndDrop::mouseUp);
    mMouseJoint = NULL;
    b2BodyDef attachDef;
    mAttachBody = physicManager().getWorld().CreateBody(&attachDef);

}

 DebugDragAndDrop::~DebugDragAndDrop()
 {
     if(mMousePos != NULL)
     {
         delete mMousePos;
     }
 }

bool DebugDragAndDrop::ReportFixture(b2Fixture *fixture)
{
    if(fixture->TestPoint(*mMousePos) && mMouseJoint == NULL)
    {
        b2Body *draggingBody = fixture->GetBody();
        b2MouseJointDef jointDef;
        jointDef.bodyA = mAttachBody;
        jointDef.bodyB = draggingBody;
        jointDef.frequencyHz = 60.0f;
        jointDef.dampingRatio = 0.0f;
        std::list<Body*> bodies = physicManager().getBodies();

        /*for(std::list<Body*>::iterator body = bodies.begin(); body != bodies.end(); ++body)
        {
            if((*body)->getb2Body()->GetType() == b2_staticBody)
            {
                jointDef.bodyA = (*body)->getb2Body();
                break;
            }
        }*/

        jointDef.target =*mMousePos;
        jointDef.maxForce = 1000.0f * draggingBody->GetMass();
        mMouseJoint = (b2MouseJoint*)physicManager().getWorld().CreateJoint(&jointDef);
        draggingBody->SetActive(true);
        return false;
    }
    return true;
}

void DebugDragAndDrop::mouseDown(const CL_InputEvent &ev, const CL_InputState &state)
{
    if(mMouseJoint == NULL)
    {
        mMousePos->Set((float) Pixels2Meters(ev.mouse_pos.x), (float) Pixels2Meters(ev.mouse_pos.y));
        b2AABB region;
        region.lowerBound.Set(mMousePos->x - 0.001f, mMousePos->y - 0.0f);
        region.upperBound.Set(mMousePos->x + 0.001f, mMousePos->y + 0.001f);
        physicManager().getWorld().QueryAABB(this, region);
    }
}

void DebugDragAndDrop::mouseMove(const CL_InputEvent &ev, const CL_InputState &state)
{
   if(mMouseJoint != NULL)
   {
       mMousePos->Set((float) Pixels2Meters(ev.mouse_pos.x), (float) Pixels2Meters(ev.mouse_pos.y));
       mMouseJoint->SetTarget(*mMousePos);
   }
}

void DebugDragAndDrop::mouseUp(const CL_InputEvent &ev, const CL_InputState &state)
{
    if(mMouseJoint != NULL)
    {
        physicManager().getWorld().DestroyJoint(mMouseJoint);
        mMouseJoint = NULL;
    }
}
