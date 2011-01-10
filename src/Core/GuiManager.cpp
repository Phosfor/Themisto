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

#include "Core/GuiManager.hpp"

GuiManager::~GuiManager()
{
    delete mWm;
    delete mWrapper;
    delete mGuiManager;
}

void GuiManager::setThemeName(const std::string &name)
{
    mThemeName = name;
}

std::string GuiManager::getThemeName()
{
    return mThemeName;
}

CL_GUIManager &GuiManager::getHandle()
{
    return *mGuiManager;
}

CL_GUIWindowManagerTexture &GuiManager::getWM()
{
    return *mWm;
}

CL_GUIComponent &GuiManager::getWrapper()
{
    return *mWrapper;
}

void GuiManager::initGuiWrapper(float width, float height)
{
    CL_GUITopLevelDescription desc("GUI");
    desc.set_size(CL_Size(width, height), false);
    desc.set_position(CL_Rect(0, 0, width, height), false);
    mWrapper = new CL_GUIComponent(mGuiManager, desc);
}

void GuiManager::initGui(CL_DisplayWindow &window, const std::string &theme)
{
    mWm = new CL_GUIWindowManagerTexture(window);
    mGuiManager = new CL_GUIManager(*mWm, theme);

    initGuiWrapper(window.get_geometry().get_width(), 
            window.get_geometry().get_height());
}
