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
            CL_Font_System::register_font("media/fonts/ubuntu.ttf", "Ubuntu");
            Client mClient;
            mClient.connect_to_server();

            CL_DisplayWindow window("Debug Visualisator", 640, 700);
            CL_GraphicContext gc = window.get_gc();
            CL_InputDevice keyboard = window.get_ic().get_keyboard();
            CL_Font font(gc, "Ubuntu", 25);

            CL_Colorf label(127/255.0f, 229/225.0f, 127/225.0f);

            while (!keyboard.get_keycode(CL_KEY_ESCAPE))
            {
                mClient.checkEvents();
                gc.clear(CL_Colorf::black);

                int offset = 20;
                for (unsigned int i=0; i < mClient.mWatchesHandles.size(); ++i)
                {
                    std::string data = 
                        "[" + mClient.mWatchesHandles[i].id + "] " +
                        mClient.mWatchesHandles[i].name + " : ";
                    font.draw_text(gc, 10, offset, data, label);

                    int width = data.length() * 10;
                    std::string value = mClient.mWatchesHandles[i].value;
                    font.draw_text(gc, 10 + width, offset, value, CL_Colorf::white);

                    offset += 25;
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
