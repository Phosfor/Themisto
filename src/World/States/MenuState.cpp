#include "World/States/MenuState.hpp"

void MenuState::init()
{
    mGC = appManager.getGraphic();

    mStatFont = new CL_Font(appManager.getGraphic(), "Ubuntu", 30);
    mBackground = CL_Image(appManager.getGraphic(), "media/ground.png");

    worldManager.initWorld();
    worldManager.enableSky(true);
    worldManager.enableMoon(true);
    worldManager.enableStars(true);
    worldManager.enableLeaves(true, 4);
    worldManager.enableClouds(true, 25);
    worldManager.enableRain(true, 50);
    worldManager.enableBirds(true);
    worldManager.setWindPower(-2.0);

    sceneLoader.loadScene();
}

void MenuState::shutdown() { }

void MenuState::update()
{
    mGC.clear();
    worldManager.update();
    mBackground.draw(mGC, 0, mGC.get_height()-mBackground.get_height());

    physicManager.step();
    physicManager.updateVisual();

    mGrass.update();

    int *time = worldManager.getWorldTime();
    mStatFont->draw_text(appManager.getGraphic(), 10, 25,
            CL_String(cl_format("world time: %1:%2:%3", time[0], time[1], time[2])), CL_Colorf::white);
    mStatFont->draw_text(appManager.getGraphic(), 10, 50,
            CL_String(cl_format("elapsed: %1", int(floor(appManager.getElapsed()+0.5)))), CL_Colorf::white);
    mStatFont->draw_text(appManager.getGraphic(), 10, 75,
            CL_String(cl_format("wind: %1", int(worldManager.getWindPower()))), CL_Colorf::white);
}

string MenuState::type()
{
    return "MenuState";
}
