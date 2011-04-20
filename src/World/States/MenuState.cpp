/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

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

#include "World/States/MenuState.hpp"

#include "Debug/DebugDraw.hpp"
#include "Debug/DebugInstruments.hpp"

void MenuState::init()
{
    mDnD = new DebugDragAndDrop;
    mDnD->init();

    mDraw = new DebugDraw;

    mDraw->SetFlags(
        b2DebugDraw::e_shapeBit //|
        //b2DebugDraw::e_jointBit |
        //b2DebugDraw::e_aabbBit  |
        //b2DebugDraw::e_pairBit
    );

    mGC = appManager().getGraphic();
    mDrawDebug = false;
    mDetailedOutput = false;
    mShowConsole = false;

    sceneLoader().loadScene("NewObjects.xml");
    physicManager().getWorld().SetDebugDraw(mDraw);
    mInputSlot = inputManager().getKeyboard().sig_key_down().connect(this, &MenuState::onKeyDown);

    initGui();
    mConsole->func_enter_pressed().set(this, &MenuState::consoleProcess);
    mConsole->func_input_released().set(this, &MenuState::consoleKeyPressed);
    mHistoryIndex = 0;
}

void MenuState::shutdown()
{
    /*delete mInfoLabel;

    delete mDropLabel;
    delete mDropSlider;

    delete mWindLabel;
    delete mWindSlider;

    delete mLeavesLabel;
    delete mLeavesSlider;*/

    delete mDnD;
    delete mDraw;
}

void MenuState::update()
{
    mGC.clear();

    worldManager().update();

    if (mDrawDebug) physicManager().getWorld().DrawDebugData();
    if (mDetailedOutput)
    {
        int *time = worldManager().getWorldTime();
        CL_Rectf camPos = levelManager().getCamera().getAbsolutePos();
        std::string information;

        information += cl_format("Elapsed: %1\n", boost::lexical_cast<std::string>(appManager().getElapsed())).c_str();
        information += cl_format("World time: %1:%2:%3\n", time[0], time[1], time[2]).c_str();
        information += cl_format("Camera abs pos: [(%1;%2), (%3;%4)]\n",
                int(camPos.left), int(camPos.top), int(camPos.right), int(camPos.bottom)).c_str();
        information += cl_format("Level name: %1\n", levelManager().getLevelName()).c_str();
        information += cl_format("Wind power: %1\n", int(worldManager().getWindPower())).c_str();

        mInfoLabel->set_text(information);

        // Enable "Control panel"
        mDropLabel->set_visible(true);
        mDropSlider->set_visible(true);

        mWindLabel->set_visible(true);
        mWindSlider->set_visible(true);

        mLeavesSlider->set_visible(true);
        mLeavesLabel->set_visible(true);
    }
    else
    {
        mInfoLabel->set_text("Press 'Ctrl+P' to view some debug information!");
        mDropLabel->set_visible(false);
        mDropSlider->set_visible(false);

        mWindLabel->set_visible(false);
        mWindSlider->set_visible(false);

        mLeavesSlider->set_visible(false);
        mLeavesLabel->set_visible(false);
    }
}

std::string MenuState::type()
{
    return "MenuState";
}

bool MenuState::consoleKeyPressed(const CL_InputEvent &key)
{
    if (mHistory.empty()) return false;

    if (key.id == CL_KEY_UP)
    {
        if ((int)mHistory.size()-mHistoryIndex-1 < 0) return false;
        std::string command = mHistory[mHistory.size()-mHistoryIndex-1];
        mConsole->set_text(command);
        mHistoryIndex++;
    }
    /*else if (key.id == CL_KEY_DOWN)
    {
        if ((int)mHistory.size()-mHistoryIndex-1 >= 0)
        {
            mConsole->set_text("");
            return false;
        }
        std::string command = mHistory[mHistory.size()-mHistoryIndex+1];
        mConsole->set_text(command);
        mHistoryIndex--;
    }*/

    return true;
}

void MenuState::consoleProcess()
{
    std::string command = mConsole->get_text();
    mHistory.push_back(command);
    std::cout << "> " << command << "\n";
    scriptsManager().runString(command);
    // Clear console text
    mConsole->set_text("");
}

void MenuState::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
    if (key.id == CL_KEY_D)
    {
        if (inputManager().keyPressed(CL_KEY_CONTROL))
        {
            mDrawDebug = !mDrawDebug;

            levelManager().getCamera().setDrawDebugData(mDrawDebug);
        }
    }

    // '~'
    if (key.id == 96)
    {
        mShowConsole = !mShowConsole;
        if (mShowConsole)
        {
            mConsoleLabel->set_visible(true);
            mConsole->set_visible(true);
            mConsole->set_focus(true);
        }
        else
        {
            mConsoleLabel->set_visible(false);
            mConsole->set_visible(false);
            mConsole->set_focus(false);
            mConsole->set_text("");
        }
    }

    if (key.id == CL_KEY_P)
    {
        if (inputManager().keyPressed(CL_KEY_CONTROL))
        {
            mDetailedOutput = !mDetailedOutput;
        }
    }

    float camSpeed = levelManager().getCamera().getSpeed();
    if (!inputManager().keyPressed(CL_KEY_CONTROL))
    {
        if (key.id == CL_KEY_RIGHT)
        {
            levelManager().getCamera().translate(-camSpeed, 0);
        }
        else if (key.id == CL_KEY_LEFT)
        {
            levelManager().getCamera().translate(+camSpeed, 0);
        }
        else if (key.id == CL_KEY_UP)
        {
            levelManager().getCamera().translate(0, +camSpeed);
        }
        else if (key.id == CL_KEY_DOWN)
        {
            levelManager().getCamera().translate(0, -camSpeed);
        }
    }
}

void MenuState::initGui()
{
    guiManager().getHandle().set_css_document(utils().getMediaFolder() + "/local.css");
    CL_Rect clientArea = appManager().getGraphic().get_cliprect();

    // Init info label
    mInfoLabel = new CL_Label(&guiManager().getWrapper());
    mInfoLabel->set_geometry(CL_Rect(clientArea.left + 5, clientArea.top + 5, CL_Size(330, 330)));
    mInfoLabel->set_class_name("info-label");
    mInfoLabel->set_text("Press 'Ctrl+P' to view some debug information!");

    // -----------------------------------------
    // Init console
    mConsoleLabel = new CL_Label(&guiManager().getWrapper());
    mConsoleLabel->set_geometry(CL_RectPS(0, clientArea.get_height()-55, clientArea.get_width(), 55));
    mConsoleLabel->set_class_name("console-label");
    mConsoleLabel->set_text(">");
    mConsoleLabel->set_visible(false);

    mConsole = new CL_LineEdit(&guiManager().getWrapper());
    mConsole->set_geometry(CL_RectPS(30, clientArea.get_height()-30, clientArea.get_width(), 30));
    mConsole->set_visible(false);
    mConsole->set_class_name("console-edit");
    // -----------------------------------------

    // Init raining slider
    mDropSlider = new CL_Slider(&guiManager().getWrapper());
    mDropSlider->set_horizontal(true);
    mDropSlider->set_visible(false);
    mDropSlider->set_position(150);
    mDropSlider->set_ranges(1, 1000, 50, 50);
    mDropSlider->set_geometry(CL_RectPS(clientArea.get_width() - 180 - 5, 5, 180, 30));

    // Rain power label
    mDropLabel = new CL_Label(&guiManager().getWrapper());
    mDropLabel->set_geometry(CL_RectPS(clientArea.get_width() - 280 - 5, 7, 100, 30));
    mDropLabel->set_visible(false);
    mDropLabel->set_class_name("info-label");
    mDropLabel->set_text("Rain power:");

    // -----------------------------------------

    // Init wind slider
    mWindSlider = new CL_Slider(&guiManager().getWrapper());
    mWindSlider->set_horizontal(true);
    mWindSlider->set_visible(false);
    mWindSlider->set_ranges(-10, 10, 2, 1);
    mWindSlider->set_position(2);
    mWindSlider->set_geometry(CL_RectPS(clientArea.get_width() - 180 - 5, 45, 180, 30));

    // Wind power label
    mWindLabel = new CL_Label(&guiManager().getWrapper());
    mWindLabel->set_geometry(CL_RectPS(clientArea.get_width() - 280 - 5, 47, 100, 30));
    mWindLabel->set_visible(false);
    mWindLabel->set_class_name("info-label");
    mWindLabel->set_text("Wind power:");

    // -----------------------------------------

    // Init leaves slider
    mLeavesSlider = new CL_Slider(&guiManager().getWrapper());
    mLeavesSlider->set_horizontal(true);
    mLeavesSlider->set_visible(false);
    mLeavesSlider->set_ranges(1, 40, 2, 1);
    mLeavesSlider->set_position(8);
    mLeavesSlider->set_geometry(CL_RectPS(clientArea.get_width() - 180 - 5, 85, 180, 30));

    // Leaves count label
    mLeavesLabel = new CL_Label(&guiManager().getWrapper());
    mLeavesLabel->set_geometry(CL_RectPS(clientArea.get_width() - 280 - 5, 87, 100, 30));
    mLeavesLabel->set_visible(false);
    mLeavesLabel->set_class_name("info-label");
    mLeavesLabel->set_text("Leaves count:");
}

void MenuState::dropNumChanged()
{
}

void MenuState::windPowerChanged()
{
}

void MenuState::leavesNumChanged()
{
}
