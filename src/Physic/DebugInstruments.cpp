#include "Physic/DebugInstruments.hpp"


void DebugDragAndDrop::init()
{
    mMousePos = new b2Vec2;
    CL_InputDevice device = inputManager.getMouse();
    device.sig_key_down().connect(DebugDragAndDrop::mouseDown);
    device.sig_pointer_move().connect(DebugDragAndDrop::mouseMove);
    device.sig_key_up().connect(DebugDagAndDrop::mouseUp);
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
    if(fixture->TestPoint(*mMousePos))
    {
        mDraggingBody = (Body*) fixture->GetBody()->GetUserData();
        mDrag = true;
        return false;
    }
    return true;
}

void DebugDragAndDrop::mouseDown(const CL_InputEvent &ev, const CL_InputState &state)
{
    mMousePos->Set((float) ev.mouse_pos.x, (float) ev.mouse_pos.y);
    b2AABB region();
    region.lowerBound = -2.0f + mMousePos;
    region.upperBound = 2.0f + mMousePos;
    physicManager.getWorld().QueryAABB(this, region);
}

void DebugDragAndDrop::mouseMove(const CL_InputEvent &ev, const CL_InputState &state)
{
    if(mDrag && mDraggingBody != NULL)
    {
        bdVec2 force =  *mMousePos - mDraggingBody->GetWorldCenter;
        mDraggingBody->ApplyForce(force, mDraggingBody->GetWorldCenter);
    }
}

void DebugDragAndDrop::mouseUp(const CL_InputEvent &ev, const CL_InputState &state)
{
    mDrag = false;
    mDragginBody = NULL;
}


