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
