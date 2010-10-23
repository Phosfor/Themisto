#ifndef _DEGUG_INSTRUMENTS_HPP
#define _DEGUG_INSTRUMENTS_HPP

#include <Box2D/Dynamics/b2World.h>
#include <Physic/Body.hpp>

class Body;

class DebugDragAndDrop: public b2QueryCallback
{
    private:
        static bool mDrag;
        static b2Vec2*  mMousePos;
        static Body* mDraggingBody;

        static void mouseDown(const CL_InputEvent &key, const CL_InputState &state);
        static void mouseUp(const CL_InputEvent &key, const CL_InputState &state);
        static void mouseMove(const CL_InputEvent &key, const CL_InputState &state);

       bool ReportFixture (b2Fixture *fixture);


    public:
        static void init();


};

#endif
