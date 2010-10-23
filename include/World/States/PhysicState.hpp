#ifndef _PHYSIC_STATE_HPP_
#define _PHYSIC_STATE_HPP_

#include "Core/StateManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/PhysicManager.hpp"
#include "Core/SceneLoader.hpp"
#include "Physic/DebugDraw.hpp"
#include "Physic/DebugInstruments.hpp"

#include <boost/foreach.hpp>

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

using namespace std;

class DebugDragAndDrop;

class PhysicState : public State
{
    private:
    CL_Font *mStatFont;
    DebugDraw mDebugDrawHandle;
    DebugDragAndDrop mDebugDragAndDropHandle;

    CL_GraphicContext mGC;
    public:

    void init();
    void shutdown();
    void update();

    string type();
};

#endif /* _PHYSIC_STATE_HPP_ */
