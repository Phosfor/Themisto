#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Debug/Visualisator/Client.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>

class DebugVisualisator
{
private:
    static void onKeyDown(const CL_InputEvent &event, const CL_InputState &state)
    {
        if (event.id == CL_MOUSE_WHEEL_DOWN)
        {
            if (topOffsetScroll - 5 >= -((numElements-3) * 25))
                topOffsetScroll -= 5;
        } else if (event.id == CL_MOUSE_WHEEL_UP)
        {
            if (topOffsetScroll + 5 <= windowHeight/25)
                topOffsetScroll += 5;
        }
    }
public:
    static int topOffsetScroll;
    static int windowHeight;
    static int numElements;

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

            CL_DisplayWindow window("Debug Visualisator", 500, 700);
            CL_GraphicContext gc = window.get_gc();
            CL_InputDevice keyboard = window.get_ic().get_keyboard();
            CL_Slot slot = window.get_ic().get_mouse().sig_key_down().connect(&DebugVisualisator::onKeyDown);
            CL_Font font(gc, "Ubuntu", 25);

            windowHeight = window.get_geometry().get_height();
            CL_Colorf label(127/255.0f, 229/225.0f, 127/225.0f);

            while (!keyboard.get_keycode(CL_KEY_ESCAPE))
            {
                mClient.checkEvents();
                numElements = mClient.mWatchesHandles.size();
                gc.clear(CL_Colorf::black);
                int offset = 20 + topOffsetScroll;

                std::vector<unsigned int> shownElements;
                for (unsigned int i=0; i < mClient.mWatchesHandles.size(); ++i)
                {
                    if (std::find(shownElements.begin(), shownElements.end(), i) != shownElements.end()) 
                        continue;

                    int offsetLeft = 10;

                    // If this is parent, add Id string for it
                    if (mClient.mWatchesHandles[i].parent == "")
                    {
                        std::string data = "[" + mClient.mWatchesHandles[i].id + "] ";
                        data += mClient.mWatchesHandles[i].name + " : ";
                        font.draw_text(gc, offsetLeft, offset, data, label);

                        int width = font.get_text_size(gc, data).width;
                        std::string value = mClient.mWatchesHandles[i].value;
                        font.draw_text(gc, offsetLeft + width, offset, value, CL_Colorf::white);

                        shownElements.push_back(i);
                        offset += 25;

                        // If this is parent, it could contain some children, find them
                        for (unsigned int j = i; j < mClient.mWatchesHandles.size(); ++j)
                        {
                            if (std::find(shownElements.begin(), shownElements.end(), j) != shownElements.end()) continue;
                            std::string id = mClient.mWatchesHandles[j].id;

                            if (mClient.mWatchesHandles[j].parent == mClient.mWatchesHandles[i].id)
                            {
                                std::string childData = mClient.mWatchesHandles[j].name + " : ";
                                font.draw_text(gc, offsetLeft + 20, offset, childData, label);

                                int childWidth = font.get_text_size(gc, childData).width;
                                std::string childValue = mClient.mWatchesHandles[j].value;
                                font.draw_text(gc, offsetLeft + childWidth + 20, offset, childValue, CL_Colorf::white);

                                shownElements.push_back(j);
                                offset += 25;
                            }
                        }
                    }
                }

                shownElements.clear();

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

int DebugVisualisator::topOffsetScroll = 0;
int DebugVisualisator::windowHeight = 0;
int DebugVisualisator::numElements = 0;
CL_ClanApplication app(&DebugVisualisator::main);
