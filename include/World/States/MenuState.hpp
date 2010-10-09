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

    CL_Label *mWindLabel;

    CL_GraphicContext mGC;
    CL_Rect mGeom;

    void rainStateChanged()
    {
       worldManager.enableRain(mRainState->is_checked(), 150);
       mWindSlider->set_enabled(mRainState->is_checked());
       mDropSlider->set_enabled(mRainState->is_checked());
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
        mBackground = CL_Image(appManager.getGraphic(), "media/ground.png");

        mRainState = new CL_CheckBox(&guiManager.getWrapper());
        mRainState->set_geometry(CL_RectPS(mGeom.get_width() - 150 - 40, 5, 150, 30));
        mRainState->set_text("Enable raining");
        mRainState->func_state_changed().set(this, &MenuState::rainStateChanged);

        mWindSlider = new CL_Slider(&guiManager.getWrapper());
        mWindSlider->set_horizontal(true);
        mWindSlider->set_position(0);
        mWindSlider->set_enabled(false);
        mWindSlider->set_ranges(-10, 10, 2, 1);
        mWindSlider->set_geometry(CL_RectPS(mGeom.get_width() - 180 - 5, 35, 180, 30));
        mWindSlider->func_value_changed().set(this, &MenuState::windPowerChanged);

        mWindLabel = new CL_Label(&guiManager.getWrapper());
        mWindLabel->set_text("wind:");
        mWindLabel->set_geometry(CL_RectPS(mGeom.get_width()-180-100, 35, 180, 30));

        mDropSlider = new CL_Slider(&guiManager.getWrapper());
        mDropSlider->set_horizontal(true);
        mDropSlider->set_enabled(false);
        mDropSlider->set_position(150);
        mDropSlider->set_ranges(1, 1000, 50, 50);
        mDropSlider->set_geometry(CL_RectPS(mGeom.get_width() - 180 - 5, 70, 180, 30));
        mDropSlider->func_value_changed().set(this, &MenuState::dropNumChanged);

        /*mSome = new CL_PushButton(&guiManager.getWrapper());
        mSome->set_geometry(CL_RectPS(mGeom.get_width() - 150 - 5, 40, 150, 30));
        mSome->set_text("Some button");*/
        //mSome->func_clicked().set(this, &MenuState::rainStateChanged);

        worldManager.initWorld();
        worldManager.enableMoon(true);
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
        delete mWindLabel;
    }

    void update()
    {
        //mGC.clear(CL_Colorf::gray);

        CL_Pointf pos1(mGeom.get_width()/2, 0);
        CL_Pointf pos2(mGeom.get_width()/2, mGeom.get_height());

        //CL_Colorf color1(14/255.0f, 33/255.0f, 78/255.0f);
        CL_Colorf color1(25/255.0f, 60/255.0f, 141/255.0f);
        CL_Colorf color2(83/255.0f, 178/255.0f, 234/255.0f);

        CL_Draw::gradient_fill(mGC, CL_Rectf(0, 0, mGeom.get_width(), mGeom.get_height()),
                CL_Gradient(color1, color2));

        worldManager.update();

        mBackground.draw(mGC, 0, mGC.get_height()-mBackground.get_height());

        int *time = worldManager.getWorldTime();
        mStatFont->draw_text(appManager.getGraphic(), 10, 25,
                CL_String(cl_format("world time: %1:%2:%3", time[0], time[1], time[2])), CL_Colorf::white);
        mStatFont->draw_text(appManager.getGraphic(), 10, 50,
                CL_String(cl_format("elapsed: %1", int(floor(appManager.getElapsed()+0.5)))), CL_Colorf::white);
        mStatFont->draw_text(appManager.getGraphic(), 10, 75,
                CL_String(cl_format("wind: %1", int(worldManager.getWindPower()))), CL_Colorf::white);
        mStatFont->draw_text(appManager.getGraphic(), 10, 100,
                CL_String(cl_format("max drops: %1", int(worldManager.getDropLimit()))), CL_Colorf::white);
    }

    string type() { return "MenuState"; }
};

#endif /* _MENU_STATE_HPP_ */
