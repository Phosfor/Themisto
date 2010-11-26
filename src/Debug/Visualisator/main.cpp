#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Debug/Visualisator/Client.hpp"

class DebugVisualisator
{
public:
    static int main(const std::vector<CL_String> &args)
    {
        CL_SetupCore setup_core;
        CL_SetupDisplay setup_display;
        CL_SetupGL setup_gl;

        try
        {
            Client mClient;
            mClient.connect_to_server();

            CL_DisplayWindow window("Debug Visualisator", 640, 480);
            CL_GraphicContext gc = window.get_gc();
            CL_InputDevice keyboard = window.get_ic().get_keyboard();
            CL_Font font(gc, "Tahoma", 20);

            while (!keyboard.get_keycode(CL_KEY_ESCAPE))
            {
                mClient.checkEvents();
                gc.clear(CL_Colorf::black);

                int offset = 10;
                std::map<std::string, watch>::const_iterator it = mClient.mWatchesHandles.begin();
                for (; it != mClient.mWatchesHandles.end(); ++it)
                {
                    std::string data = mClient.mWatchesHandles[it->first].name + " : " +
                        mClient.mWatchesHandles[it->first].value;
                    font.draw_text(gc, 10, offset, data, CL_Colorf::white);
                    offset += 20;
                }

                window.flip();
                CL_KeepAlive::process();
                CL_System::sleep(10);
            }
        }
        catch(CL_Exception &exception)
        {
            CL_ConsoleWindow console("Console", 80, 160);
            CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
            console.display_close_message();
            return -1;
        }

        return 0;
    }
};

CL_ClanApplication app(&DebugVisualisator::main);
