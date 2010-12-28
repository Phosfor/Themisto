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
