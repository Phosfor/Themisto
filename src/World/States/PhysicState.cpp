#include "World/States/PhysicState.hpp"

void PhysicState::init()
{
    mGC = appManager.getGraphic();

    mStatFont = new CL_Font(appManager.getGraphic(), "Ubuntu", 30);
    mBackground = CL_Image(appManager.getGraphic(), "media/ground.png");

    sceneLoader.loadScene();
}

void PhysicState::shutdown() { }

void PhysicState::update()
{
    mGC.clear(CL_Colorf::grey);
    //mBackground.draw(mGC, 0, mGC.get_height()-mBackground.get_height());

    physicManager.step();
    physicManager.updateVisual();

    mStatFont->draw_text(appManager.getGraphic(), 10, 25,
            CL_String(cl_format("elapsed: %1", int(floor(appManager.getElapsed()+0.5)))), CL_Colorf::white);
}

string PhysicState::type()
{
    return "PhysicState";
}
