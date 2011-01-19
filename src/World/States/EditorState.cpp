/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

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

#include "World/States/EditorState.hpp"

#include "Debug/DebugDraw.hpp"
#include "Debug/DebugInstruments.hpp"

void EditorState::init()
{
    mDnD = new DebugDragAndDrop;
    mDnD->init();

    mDraw = new DebugDraw;

    mDraw->SetFlags(
        b2DebugDraw::e_shapeBit //|
        //b2DebugDraw::e_jointBit |
        //b2DebugDraw::e_aabbBit  |
        //b2DebugDraw::e_pairBit
    );

    mGC = appManager().getGraphic();
    mDrawDebug = false;

    sceneLoader().loadScene("test.xml");
    physicManager().getWorld().SetDebugDraw(mDraw);
    mInputSlot = inputManager().getKeyboard().sig_key_down().connect(this, &EditorState::onKeyDown);

    mEnvironEnabled = environManager().getEnvironEnabled();
}

void EditorState::shutdown()
{
    delete mDnD;
    delete mDraw;
}

void EditorState::update()
{
    mGC.clear();

    physicManager().step();
    if (mEnvironEnabled) environManager().update();

    if (mDrawDebug) physicManager().getWorld().DrawDebugData();
}

std::string EditorState::type()
{
    return "EditorState";
}


void EditorState::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
    if (key.id == CL_KEY_D)
    {
        if (inputManager().keyPressed(CL_KEY_CONTROL))
        {
            mDrawDebug = !mDrawDebug;
        }
    }

    float camSpeed = levelManager().getCameraSpeed();
    if (key.id == CL_KEY_RIGHT)
    {
        levelManager().translateCamera(-camSpeed, 0);
    }
    else if (key.id == CL_KEY_LEFT)
    {
        levelManager().translateCamera(+camSpeed, 0);
    }
    else if (key.id == CL_KEY_UP)
    {
        levelManager().translateCamera(0, +camSpeed);
    }
    else if (key.id == CL_KEY_DOWN)
    {
        levelManager().translateCamera(0, -camSpeed);
    }
}
