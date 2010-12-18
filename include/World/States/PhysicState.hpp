/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _PHYSIC_STATE_HPP_
#define _PHYSIC_STATE_HPP_

#include "Core/StateManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/PhysicManager.hpp"
#include "Core/SceneLoader.hpp"
#include "Debug/DebugDraw.hpp"
#include "Debug/DebugInstruments.hpp"
#include "Debug/DebugWatcher.hpp"
#include "Debug/DebugIO.hpp"
#include "Debug/PhysicTests.hpp"

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

class DebugDragAndDrop;
class PhysicState : public State
{
    private:
    CL_Font *mStatFont;
    DebugDraw mDebugDrawHandle;
    DebugDragAndDrop *mDebugDragAndDropHandle;

    CL_GraphicContext mGC;
    public:

    void init();
    void shutdown();
    void update();

    std::string type();
};

#endif /* _PHYSIC_STATE_HPP_ */
