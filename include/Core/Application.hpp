#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

#include "Managers/ConfigManager.hpp"
#include "Managers/LogManager.hpp"

class GameApplication
{
    public:
    static int main(const std::vector<CL_String> &args)
    {
        // Main environment handlers
        CL_SetupCore coreHandle;
        CL_SetupDisplay displayHandle;
        CL_SetupGL glHandle;

        try
        {
            short width = configManager.getValue<int>("window.width", 640);
            short height = configManager.getValue<int>("window.height", 480);

            CL_DisplayWindow window("Themisto", width, height);
            LOG("The window has been created!");

            CL_GraphicContext gc = window.get_gc();
            CL_InputDevice keyboard = window.get_ic().get_keyboard();

            while (!keyboard.get_keycode(CL_KEY_ESCAPE))
            {
                gc.clear(CL_Colorf::gray);

                window.flip();
                CL_KeepAlive::process();
                CL_System::sleep(10);
            }
        }
        catch(CL_Exception &error)
        {
            CL_ConsoleWindow console("Themisto console", 80, 160);
            CL_Console::write_line("Exception happened: " + error.get_message_and_stack_trace());
            console.display_close_message();
            return -1;
        }

        return 0;
    }
};
