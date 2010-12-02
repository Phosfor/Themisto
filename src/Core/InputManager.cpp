/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Core/InputManager.hpp"

void InputManager::initInput()
{
    mInput = appManager.getWindow().get_ic();

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
