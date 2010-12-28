/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include <ClanLib/gl.h>
#include <ClanLib/gl1.h>
#include <ClanLib/swrender.h>

#include "Debug/Visualisator/Client.hpp"
#include "Core/ConfigManager.hpp"
#include "Core/GuiManager.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>

class DebugVisualisator
{
public:
    static int main(const std::vector<CL_String> &args)
    {
        CL_SetupCore setup_core;
        CL_SetupDisplay setup_display;
        CL_SetupGL gl2Handle;
        CL_SetupGL1 gl1Handle;
        CL_SetupSWRender swHandle;

        try
        {
            // Set software render as default
            swHandle.set_current();
            std::string render = configManager().getValue<std::string>("window.render", "software");
            if (render == "opengl1")
                gl1Handle.set_current();
            else if (render == "opengl2")
                gl2Handle.set_current();

            CL_Font_System::register_font("media/fonts/ubuntu.ttf", "Ubuntu");
            Client mClient;
            mClient.connect_to_server();

            CL_DisplayWindow window("Debug Visualisator", 500, 700);

            guiManager().initGui(window, "media/gui_basic");
            CL_GUIManager gui = guiManager().getHandle();
            CL_GUIWindowManagerTexture wm = guiManager().getWM();

            CL_GraphicContext gc = window.get_gc();
            CL_InputDevice keyboard = window.get_ic().get_keyboard();
            CL_Font font(gc, "Ubuntu", 25);

            CL_ListView *listView = new CL_ListView(&guiManager().getWrapper());
            listView->set_geometry(CL_Rectf(0, 0, 500, 700));
            //listView->set_display_mode(listview_mode_details);

            CL_ListViewHeader *lv_header = listView->get_header();
            lv_header->append(lv_header->create_column("col1_id", "Command")).set_width(400);
            lv_header->append(lv_header->create_column("col2_id", "Value")).set_width(100);

            CL_ListViewItem doc_item = listView->get_document_item();

            CL_Colorf label(127/255.0f, 229/225.0f, 127/225.0f);

            while (!keyboard.get_keycode(CL_KEY_ESCAPE))
            {
                mClient.checkEvents();
                gc.clear(CL_Colorf::grey);

                listView->clear();
                std::vector<unsigned int> shownElements;
                for (unsigned int i=0; i < mClient.mWatchesHandles.size(); ++i)
                {
                    if (std::find(shownElements.begin(), shownElements.end(), i) != shownElements.end()) 
                        continue;

                    //If this is parent, add Id string for it
                    if (mClient.mWatchesHandles[i].parent == "")
                    {
                        CL_ListViewItem parent = listView->create_item();
                        {
                            std::string data = "[" + mClient.mWatchesHandles[i].id + "] ";
                            data += mClient.mWatchesHandles[i].name;
                            parent.set_column_text("col1_id", data);
                            parent.set_column_text("col2_id", mClient.mWatchesHandles[i].value);
                            parent.set_open(true);
                            doc_item.append_child(parent);
                        }

                        shownElements.push_back(i);

                        //If this is parent, it could contain some children, find them
                        for (unsigned int j = i; j < mClient.mWatchesHandles.size(); ++j)
                        {
                            if (std::find(shownElements.begin(), shownElements.end(), j) != shownElements.end()) continue;
                            std::string id = mClient.mWatchesHandles[j].id;

                            if (mClient.mWatchesHandles[j].parent == mClient.mWatchesHandles[i].id)
                            {
                                CL_ListViewItem child = listView->create_item();
                                child.set_column_text("col1_id", mClient.mWatchesHandles[j].name);
                                child.set_column_text("col2_id", mClient.mWatchesHandles[j].value);
                                child.set_open(true);
                                parent.append_child(child);

                                shownElements.push_back(j);
                            }
                        }
                    }
                }

                shownElements.clear();

                wm.process();
                wm.draw_windows(gc);

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
