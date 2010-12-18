/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/States/PhysicState.hpp"

void PhysicState::init()
{
    mDebugDragAndDropHandle = new DebugDragAndDrop;
    mDebugDragAndDropHandle->init();

    mGC = appManager().getGraphic();
    mStatFont = new CL_Font(appManager().getGraphic(), "Ubuntu", 30);

    areaManager().init(&physicManager().getWorld(), 20);
    sceneLoader().loadScene("testingPhysic.xml");

    mDebugDrawHandle.SetFlags(
            b2DebugDraw::e_shapeBit //|
            //b2DebugDraw::e_jointBit |
            //b2DebugDraw::e_aabbBit  |
            //b2DebugDraw::e_pairBit
            );
    physicManager().getWorld().SetDebugDraw(&mDebugDrawHandle);

    debugWatcher().init();
    debugIO().init();
    physicTests().RunTest();
}


void PhysicState::shutdown()
{
    delete mStatFont;
    delete mDebugDragAndDropHandle;
}

void PhysicState::update()
{
    mGC.clear(CL_Colorf::black);

    objectsManager().update();
    physicManager().step();
    physicManager().getWorld().DrawDebugData();

    mStatFont->draw_text(appManager().getGraphic(), 10, 25,
            CL_String(cl_format("elapsed: %1", int(floor(appManager().getElapsed()+0.5)))), CL_Colorf::white);

    debugIO().step();
    debugWatcher().step();
}

string PhysicState::type()
{
    return "PhysicState";
}
