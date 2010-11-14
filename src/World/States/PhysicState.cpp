#include "World/States/PhysicState.hpp"

void PhysicState::init()
{
    mDebugDragAndDropHandle = new DebugDragAndDrop;
    mDebugDragAndDropHandle->init();

    mGC = appManager.getGraphic();
    mStatFont = new CL_Font(appManager.getGraphic(), "Ubuntu", 30);
    
    areaManager.init(&physicManager.getWorld(), 20);
    sceneLoader.loadScene("testingPhysic.xml");

    mDebugDrawHandle.SetFlags(
            b2DebugDraw::e_shapeBit //|
            //b2DebugDraw::e_jointBit |
            //b2DebugDraw::e_aabbBit  |
            //b2DebugDraw::e_pairBit
            );
    physicManager.getWorld().SetDebugDraw(&mDebugDrawHandle);


}


void PhysicState::shutdown()
{
    delete mStatFont;
    delete mDebugDragAndDropHandle;
}

void PhysicState::update()
{
    mGC.clear(CL_Colorf::black);

    physicManager.step();
    physicManager.updateVisual();
    physicManager.getWorld().DrawDebugData();

    mStatFont->draw_text(appManager.getGraphic(), 10, 25,
            CL_String(cl_format("elapsed: %1", int(floor(appManager.getElapsed()+0.5)))), CL_Colorf::white);
}

string PhysicState::type()
{
    return "PhysicState";
}
