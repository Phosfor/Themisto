#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

#include "Managers/ConfigManager.hpp"
#include "Managers/LogManager.hpp"
#include "Managers/FrameManager.hpp"

class GameApplication
{
    public:
        static int main(const std::vector<CL_String> &args);

    private:
        static void onInput(const CL_InputEvent &key, const CL_InputState &state);
        static void onWindowClose();

};
