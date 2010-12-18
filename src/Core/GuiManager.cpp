/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Core/GuiManager.hpp"

GuiManager::~GuiManager()
{
    delete mGuiManager;
    delete mWm;
    delete mWrapper;
}

void GuiManager::setThemeName(const std::string &name)
{
    mThemeName = name;
}

std::string GuiManager::getThemeName()
{
    return mThemeName;
}

CL_GUIManager GuiManager::getHandle()
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
