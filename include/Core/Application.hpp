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
    static int main(const std::vector<CL_String> &args)
    {
        // Main environment handlers
        CL_SetupCore coreHandle;
        LOG("Core handle is initialized");
        CL_SetupDisplay displayHandle;
        LOG("Display handle is initialized");
        CL_SetupGL glHandle;
        LOG("GL handle is initialized");

        try
        {
            short width = configManager.getValue<int>("window.width", 640);
            short height = configManager.getValue<int>("window.height", 480);
            bool fullscreen = configManager.getValue<bool>("window.fullscreen", false);

            CL_DisplayWindow window("Themisto", width, height, fullscreen);
            LOG("The window has been created!");

            CL_GraphicContext gc = window.get_gc();
            CL_InputDevice keyboard = window.get_ic().get_keyboard();

            /*TODO: Move to resoruce loader */
            CL_Font_System::register_font("media/pneumatics.ttf", "Pneumatics");
            CL_Font font(gc, "Pneumatics", 40);

            /*TODO: Change app exit code */
            while (!keyboard.get_keycode(CL_KEY_ESCAPE))
            {
                frameManager.frameStarted();
                gc.clear(CL_Colorf::gray);

                font.draw_text(gc, 10, 70, CL_String(cl_format("fps: %1", frameManager.getFps())), CL_Colorf::black);
                font.draw_text(gc, 10, 140, CL_String(cl_format("elapsed: %1", frameManager.getElapsed())), CL_Colorf::black);

                CL_KeepAlive::process();
                /*TODO: wtf! (check vsync) */
                /*
                 * CL_Display::flip()   - Locks to the default frame rate
                 * CL_Display::flip(-1) - Locks to the default frame rate
                 * CL_Display::flip(0)  - Do not lock to the frame rate
                 * CL_Display::flip(1)  - Sync to every frame
                 * CL_Display::flip(2)  - Sync to every 2nd frame
                */
                window.flip(0);
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
