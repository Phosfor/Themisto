#include "Core/GuiManager.hpp"

GuiManager::~GuiManager()
{
    delete mGuiManager;
}

void GuiManager::setThemeName(const string &name)
{
    mThemeName = name;
}

string GuiManager::getThemeName()
{
    return mThemeName;
}

CL_GUIManager GuiManager::getHandle()
{
    return *mGuiManager;
}

void GuiManager::initGui(CL_GUIWindowManagerTexture &wm, const string &theme)
{
    mGuiManager = new CL_GUIManager(wm, theme);
}
