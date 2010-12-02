#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>
#include <ClanLib/sound.h>

#include "Core/LogManager.hpp"
#include "Core/ConfigManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/StateManager.hpp"
#include "Core/GuiManager.hpp"
#include "Core/Utils.hpp"
#include "Core/InputManager.hpp"
#include "World/Objects/TypesManager.hpp"

#include "World/States/MenuState.hpp"
#include "World/States/PhysicState.hpp"

// This is wrapper to make the Main function crossplatform
class Application
{
    public:
        static int main(const std::vector<CL_String> &args);

    private:
        // Slots
        static void onInput(const CL_InputEvent &key, const CL_InputState &state);
        static void onWindowClose();
        static void doNothing();
};

#endif /* _APPLICATION_HPP_ */
