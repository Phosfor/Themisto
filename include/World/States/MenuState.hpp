/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _MENU_STATE_HPP_
#define _MENU_STATE_HPP_

#include "Core/StateManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/SceneLoader.hpp"
#include "Core/PhysicManager.hpp"

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

#include "Debug/DebugDraw.hpp"
#include "Debug/DebugInstruments.hpp"

class DebugDragAndDrop;
class MenuState : public State
{
    private:
    CL_Font *mStatFont;
    bool mEnvironEnabled;
    DebugDraw mDebugDrawHandle;
    DebugDragAndDrop *mDebugDragAndDropHandle;
    CL_GraphicContext mGC;

    // Remove this later
    CL_Image ground;

    void init();
    void shutdown();
    void update();

    std::string type();
};

#endif /* _MENU_STATE_HPP_ */
