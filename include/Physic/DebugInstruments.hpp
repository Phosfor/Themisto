#ifndef _DEGUG_INSTRUMENTS_HPP
#define _DEGUG_INSTRUMENTS_HPP

#include <Box2D/Dynamics/b2World.h>
#include <Physic/Body.hpp>
#include "Core/ApplicationManager.hpp"

class DebugDragAndDrop: public b2QueryCallback
{
    private:
        bool mDrag;
        b2Vec2*  mMousePos;
        Body* mDraggingBody;

        void mouseDown(const CL_InputEvent &key, const CL_InputState &state);
        void mouseUp(const CL_InputEvent &key, const CL_InputState &state);
        void mouseMove(const CL_InputEvent &key, const CL_InputState &state);

        bool ReportFixture (b2Fixture *fixture);


    public:
        void init();
        ~DebugDragAndDrop();

};

#endif
