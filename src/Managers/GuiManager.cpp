#include "Managers/GuiManager.hpp"

void GuiManager::setThemeName(const string &name)
{
    mThemeName = name;
}

string GuiManager::getThemeName()
{
    return mThemeName;
}

CL_GUIManager *GuiManager::getHandle()
{
    return mGuiManager;
}
