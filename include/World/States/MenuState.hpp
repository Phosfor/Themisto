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

    void init()
    {
        mStatFont = new CL_Font(appManager.getGraphic(), "Ubuntu", 30);

        mBackground = CL_Image(appManager.getGraphic(), "media/tree.png");

        worldManager.initWorld();
        worldManager.enableMoon(true, 0.2, 0.2);
        worldManager.enableRain(true, 150);
        worldManager.setWindPower(-5.0);
    }

    void shutdown()
    {
        delete mStatFont;
    }

    void update()
    {
        appManager.getGraphic().clear(CL_Colorf::gray);
        mBackground.draw(appManager.getGraphic(), 0, 0);

        worldManager.update();

        int *time = worldManager.getWorldTime();
        mStatFont->draw_text(appManager.getGraphic(), 10, 25,
                CL_String(cl_format("world time: %1:%2:%3", time[0], time[1], time[2])), CL_Colorf::white);
        mStatFont->draw_text(appManager.getGraphic(), 10, 50,
                CL_String(cl_format("elapsed: %1", floor(appManager.getElapsed()+0.5))), CL_Colorf::white);
        mStatFont->draw_text(appManager.getGraphic(), 10, 75,
                CL_String(cl_format("wind: %1", floor(worldManager.getWindPower()))), CL_Colorf::white);
    }

    string type() { return "MenuState"; }
};

#endif /* _MENU_STATE_HPP_ */
