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

        CL_InputDevice mKeyboard;
        CL_InputDevice mMouse;
        CL_InputDevice mJoystick;

    public:
        ~InputManager();
        void initInput();

        CL_InputContext &getInput();
        CL_InputDevice &getKeyboard();
        CL_InputDevice &getMouse();
        CL_InputDevice &getJoystick();
};

#endif /* _INPUT_MANAGER_HPP_ */
