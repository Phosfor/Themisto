/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

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

#ifndef _EDITOR_STATE_HPP_
#define _EDITOR_STATE_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

#include "Core/StateManager.hpp"
#include "Core/GuiManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/SceneLoader.hpp"
#include "Core/PhysicManager.hpp"
#include "Core/LevelManager.hpp"


class DebugDragAndDrop;
class DebugDraw;
class EditorState : public State
{
    private:
        DebugDraw *mDraw;
        DebugDragAndDrop *mDnD;

        CL_GraphicContext mGC;

        // Remove this later
        void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);
        CL_Slot mInputSlot;
        bool mDrawDebug;

        void init();
        void shutdown();
        void update();

        std::string type();
};

#endif /* _EDITOR_STATE_HPP_ */
