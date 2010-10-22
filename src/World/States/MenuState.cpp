#include "World/States/MenuState.hpp"

void MenuState::init()
{
    mGC = appManager.getGraphic();

    mStatFont = new CL_Font(appManager.getGraphic(), "Ubuntu", 30);
    mBackground = CL_Image(appManager.getGraphic(), "media/ground.png");

    environManager.initEnviron();
    environManager.enableType(true, EnvironManager::Environ_Sky);
    environManager.enableType(true, EnvironManager::Environ_Moon);
    environManager.enableType(true, EnvironManager::Environ_Stars);
    environManager.enableType(true, EnvironManager::Environ_Leaves, 4);
    environManager.enableType(true, EnvironManager::Environ_Clouds, 10);
    environManager.enableType(true, EnvironManager::Environ_Rain, 40);
    environManager.enableType(true, EnvironManager::Environ_Birds);
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
