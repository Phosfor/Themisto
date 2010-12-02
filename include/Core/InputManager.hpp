/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _INPUT_MANAGER_HPP_
#define _INPUT_MANAGER_HPP_

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include "Core/ApplicationManager.hpp"

#include <boost/serialization/singleton.hpp>

#define inputManager (InputManager::get_mutable_instance())
#define inputManagerConst (InputManager::get_const_instance())

class InputManager : public boost::serialization::singleton<InputManager>
{
    private:
        CL_InputContext mInput;

        CL_InputDevice *mKeyboard;
        CL_InputDevice *mMouse;
        CL_InputDevice *mJoystick;

    public:
        void initInput();

        CL_InputContext &getInput();
        CL_InputDevice &getKeyboard();
        CL_InputDevice &getMouse();
        CL_InputDevice &getJoystick();

        // Could be used with mouse buttons too
        bool keyPressed(int code);

        CL_Point getMousePos();
        void setMousePos(int x, int y);
};

#endif /* _INPUT_MANAGER_HPP_ */
