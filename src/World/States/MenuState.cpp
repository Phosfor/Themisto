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

#include "Debug/DebugDraw.hpp"
#include "Debug/DebugInstruments.hpp"

void MenuState::init()
{
    mDnD = new DebugDragAndDrop;
    mDnD->init();

    mDraw->SetFlags(
        b2DebugDraw::e_shapeBit //|
        //b2DebugDraw::e_jointBit |
        //b2DebugDraw::e_aabbBit  |
        //b2DebugDraw::e_pairBit
    );


    mGC = appManager().getGraphic();

    areaManager().init(&physicManager().getWorld(), 20);
    sceneLoader().loadScene("test.xml");
    physicManager().getWorld().SetDebugDraw(mDraw);

    mEnvironEnabled = environManager().getEnvironEnabled();

    ground = CL_Image(mGC, "media/ground.png");
}

void MenuState::shutdown() 
{
    delete mDnD;
    delete mDraw;
}

void MenuState::update()
{
    mGC.clear();
    if (mEnvironEnabled) environManager().update();

    objectsManager().update();
    physicManager().step();
    ground.draw(mGC, 0, 770);

    // TODO: Bind some key to enable debug drawing
    //physicManager().getWorld().DrawDebugData();
}

std::string MenuState::type()
{
    return "MenuState";
}
