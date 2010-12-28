/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

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

std::string PhysicState::type()
{
    return "PhysicState";
}
