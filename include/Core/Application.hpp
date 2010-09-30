#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include "Managers/ConfigManager.hpp"
#include "Managers/LogManager.hpp"
#include "Managers/ApplicationManager.hpp"
#include "Managers/ResourceManager.hpp"
#include "Managers/StateManager.hpp"
#include "Managers/GuiManager.hpp"
#include "Core/Utils.hpp"
#include "Managers/States/MenuState.hpp"

class GameApplication
{
    public:
        static int main(const std::vector<CL_String> &args);

    private:
        // Slots
        static void onInput(const CL_InputEvent &key, const CL_InputState &state);
        static void onWindowClose();
};
