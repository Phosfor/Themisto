#ifndef _DEGUG_INSTRUMENTS_HPP
#define _DEGUG_INSTRUMENTS_HPP

#include <Box2D/Dynamics/b2World.h>
#include "World/Objects/Body.hpp"
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
