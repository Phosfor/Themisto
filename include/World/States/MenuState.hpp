#ifndef _MENU_STATE_HPP_
#define _MENU_STATE_HPP_

#include "Core/StateManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "World/WorldManager.hpp"

#include <boost/foreach.hpp>

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

using namespace std;

class MenuState : public State
{
    CL_Font *mStatFont;
    CL_Image mBackground;

    CL_CheckBox *mRainState;
    CL_PushButton *mSome;
    CL_Slider *mWindSlider;
    CL_Slider *mDropSlider;

    CL_GraphicContext mGC;
    CL_Rect mGeom;

    void rainStateChanged()
    {
       worldManager.enableRain(mRainState->is_checked(), 150);
       mWindSlider->set_enabled(mRainState->is_checked());
    }

    void windPowerChanged()
    {
        worldManager.setWindPower(mWindSlider->get_position());
    }

    void dropNumChanged()
    {
        worldManager.setDropLimit(mDropSlider->get_position());
    }

    void init()
    {
        mGC = appManager.getGraphic();
        mGeom = appManager.getWindow().get_geometry();

        mStatFont = new CL_Font(appManager.getGraphic(), "Ubuntu", 30);
        mBackground = CL_Image(appManager.getGraphic(), "media/tree.png");

        mRainState = new CL_CheckBox(&guiManager.getWrapper());
        mRainState->set_geometry(CL_RectPS(mGeom.get_width() - 150 - 5, 5, 150, 30));
        mRainState->set_text("Enable raining");
        mRainState->func_state_changed().set(this, &MenuState::rainStateChanged);

        mWindSlider = new CL_Slider(&guiManager.getWrapper());
        mWindSlider->set_horizontal(true);
        mWindSlider->set_position(0);
        mWindSlider->set_ranges(-10, 10, 2, 1);
        mWindSlider->set_geometry(CL_RectPS(mGeom.get_width() - 180 - 5, 40, 180, 30));
        mWindSlider->func_value_changed().set(this, &MenuState::windPowerChanged);

        mDropSlider = new CL_Slider(&guiManager.getWrapper());
        mDropSlider->set_horizontal(true);
        mDropSlider->set_position(150);
        mDropSlider->set_ranges(1, 1000, 50, 50);
        mDropSlider->set_geometry(CL_RectPS(mGeom.get_width() - 180 - 5, 75, 180, 30));
        mDropSlider->func_value_changed().set(this, &MenuState::dropNumChanged);

        /*mSome = new CL_PushButton(&guiManager.getWrapper());
        mSome->set_geometry(CL_RectPS(mGeom.get_width() - 150 - 5, 40, 150, 30));
        mSome->set_text("Some button");*/
        //mSome->func_clicked().set(this, &MenuState::rainStateChanged);

        worldManager.initWorld();
        worldManager.enableMoon(true, 0.2, 0.2);
        //worldManager.enableRain(true, 150);
        //worldManager.setWindPower(-5.0);
    }

    void shutdown()
    {
        delete mStatFont;
        delete mRainState;
        delete mSome;
        delete mWindSlider;
        delete mDropSlider;
    }

    void update()
    {
        mGC.clear(CL_Colorf::gray);
        mBackground.draw(mGC, 0, 0);

        worldManager.update();

        int *time = worldManager.getWorldTime();
        mStatFont->draw_text(appManager.getGraphic(), 10, 25,
                CL_String(cl_format("world time: %1:%2:%3", time[0], time[1], time[2])), CL_Colorf::white);
        mStatFont->draw_text(appManager.getGraphic(), 10, 50,
                CL_String(cl_format("elapsed: %1", floor(appManager.getElapsed()+0.5))), CL_Colorf::white);
        mStatFont->draw_text(appManager.getGraphic(), 10, 75,
                CL_String(cl_format("wind: %1", worldManager.getWindPower())), CL_Colorf::white);
        mStatFont->draw_text(appManager.getGraphic(), 10, 100,
                CL_String(cl_format("max drops: %1", worldManager.getDropLimit())), CL_Colorf::white);
    }

    string type() { return "MenuState"; }
};

#endif /* _MENU_STATE_HPP_ */
