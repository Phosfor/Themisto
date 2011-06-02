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

#pragma once

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include "Core/ApplicationManager.hpp"

#include <boost/serialization/singleton.hpp>

class InputManager : public boost::serialization::singleton<InputManager>
{
    private:
        CL_InputContext mInput;

        CL_InputDevice *mKeyboard;
        CL_InputDevice *mMouse;
        CL_InputDevice *mJoystick;

        std::vector<CL_Slot> mSlots;

    public:
        void initInput();

        CL_InputContext &getInput();
        CL_InputDevice &getKeyboard();
        CL_InputDevice &getMouse();
        CL_InputDevice &getJoystick();

        std::vector<CL_Slot> &getSlots();

        // Could be used with mouse buttons too
        bool keyPressed(int code);

        CL_Point getMousePos();
        void setMousePos(int x, int y);
};

inline InputManager &inputManager() { return InputManager::get_mutable_instance(); }
