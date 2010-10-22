#include "World/States/MenuState.hpp"

void MenuState::init()
{
    mGC = appManager.getGraphic();

    mStatFont = new CL_Font(appManager.getGraphic(), "Ubuntu", 30);
    mBackground = CL_Image(appManager.getGraphic(), "media/ground.png");

    environManager.initEnviron();
    environManager.enableSky(true);
    environManager.enableMoon(true);
    environManager.enableStars(true);
    environManager.enableLeaves(true, 4);
    environManager.enableClouds(true, 15);
    environManager.enableRain(true, 50);
    environManager.enableBirds(true);
    environManager.setWindPower(-2.0);
}

void MenuState::shutdown() { }

void MenuState::update()
{
    mGC.clear();
    environManager.update();
    mBackground.draw(mGC, 0, mGC.get_height()-mBackground.get_height());

    mGrass.update();

    int *time = environManager.getEnvironTime();
    mStatFont->draw_text(appManager.getGraphic(), 10, 25,
            CL_String(cl_format("world time: %1:%2:%3", time[0], time[1], time[2])), CL_Colorf::white);
    mStatFont->draw_text(appManager.getGraphic(), 10, 50,
            CL_String(cl_format("elapsed: %1", int(floor(appManager.getElapsed()+0.5)))), CL_Colorf::white);
    mStatFont->draw_text(appManager.getGraphic(), 10, 75,
            CL_String(cl_format("wind: %1", int(environManager.getWindPower()))), CL_Colorf::white);
}

string MenuState::type()
{
    return "MenuState";
}
