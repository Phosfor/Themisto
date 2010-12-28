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

#include "World/States/MenuState.hpp"

void MenuState::init()
{
    mDebugDragAndDropHandle = new DebugDragAndDrop;
    mDebugDragAndDropHandle->init();

    mGC = appManager().getGraphic();
    mStatFont = new CL_Font(appManager().getGraphic(), "Ubuntu", 30);

    areaManager().init(&physicManager().getWorld(), 20);
    sceneLoader().loadScene("test.xml");

    mDebugDrawHandle.SetFlags(
            b2DebugDraw::e_shapeBit //|
            //b2DebugDraw::e_jointBit |
            //b2DebugDraw::e_aabbBit  |
            //b2DebugDraw::e_pairBit
            );

    physicManager().getWorld().SetDebugDraw(&mDebugDrawHandle);

    mEnvironEnabled = environManager().getEnvironEnabled();

    ground = CL_Image(mGC, "media/ground.png");
}

void MenuState::shutdown() 
{
    delete mStatFont;
}

void MenuState::update()
{
    mGC.clear();
    if (mEnvironEnabled) environManager().update();

    objectsManager().update();
    physicManager().step();
    ground.draw(mGC, 0, 770);
    physicManager().getWorld().DrawDebugData();


    int *time = environManager().getEnvironTime();
    mStatFont->draw_text(appManager().getGraphic(), 10, 25,
            CL_String(cl_format("world time: %1:%2:%3", time[0], time[1], time[2])), CL_Colorf::white);
    mStatFont->draw_text(appManager().getGraphic(), 10, 50,
            CL_String(cl_format("elapsed: %1", int(floor(appManager().getElapsed()+0.5)))), CL_Colorf::white);
    mStatFont->draw_text(appManager().getGraphic(), 10, 75,
            CL_String(cl_format("wind: %1", int(environManager().getWindPower()))), CL_Colorf::white);
}

std::string MenuState::type()
{
    return "MenuState";
}
