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

#include "Core/InputManager.hpp"

void InputManager::initInput()
{
    mInput = appManager().getWindow().get_ic();

    mKeyboard = &mInput.get_keyboard();
    mMouse = &mInput.get_mouse();
    mJoystick = &mInput.get_joystick();
}

CL_InputContext &InputManager::getInput()
{
    return mInput;
}

CL_InputDevice &InputManager::getKeyboard()
{
    return *mKeyboard;
}

CL_InputDevice &InputManager::getMouse()
{
    return *mMouse;
}

CL_InputDevice &InputManager::getJoystick()
{
    return *mJoystick;
}

bool InputManager::keyPressed(int code)
{
    return mKeyboard->get_keycode(code);
}

CL_Point InputManager::getMousePos()
{
    return mMouse->get_position();
}

void InputManager::setMousePos(int x, int y)
{
    mMouse->set_position(x, y);
}

std::vector<CL_Slot> &InputManager::getSlots()
{
    return mSlots;
}
