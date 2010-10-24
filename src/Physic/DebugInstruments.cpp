#include "Physic/DebugInstruments.hpp"

void DebugDragAndDrop::init()
{
    cout<<"\nInit!\n";
    mMousePos = new b2Vec2;
    CL_InputDevice device = inputManager.getMouse();
    device.sig_key_down().connect(this, &DebugDragAndDrop::mouseDown);
    device.sig_pointer_move().connect(this, &DebugDragAndDrop::mouseMove);
    device.sig_key_up().connect(this, &DebugDragAndDrop::mouseUp);
    cout<<"\nEnd init!\n";
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
        cout<<"\nDrag!\n";
        return false;
    }
    return true;
}

void DebugDragAndDrop::mouseDown(const CL_InputEvent &ev, const CL_InputState &state)
{
    cout<<"\nMouseDown!\n";
    mMousePos->Set((float) ev.mouse_pos.x, (float) ev.mouse_pos.y);
    b2AABB region;
    region.lowerBound.Set(mMousePos->x - 2.0f, mMousePos->y - 2.0f);
    region.upperBound.Set(mMousePos->x + 2.0f, mMousePos->y + 2.0f);
    physicManager.getWorld().QueryAABB(this, region);
}

void DebugDragAndDrop::mouseMove(const CL_InputEvent &ev, const CL_InputState &state)
{
    cout<<"\nMouseMove!\n";
    if(mDrag && mDraggingBody != NULL)
    {
        b2Vec2 force =  *mMousePos - mDraggingBody->getb2Body().GetWorldCenter();
        mDraggingBody->getb2Body().ApplyForce(force, mDraggingBody->getb2Body().GetWorldCenter());
    }
}

void DebugDragAndDrop::mouseUp(const CL_InputEvent &ev, const CL_InputState &state)
{
     cout<<"\nMouseUp!\n";
    mDrag = false;
    mDraggingBody = NULL;
}


