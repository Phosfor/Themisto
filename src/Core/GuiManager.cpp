#include "Core/GuiManager.hpp"

/*void GuiManager::~GuiManager()
{
    delete mGuiManager;
}*/

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

/*void GuiManager::initGui(const string &theme)
{
    mGuiManager = new mGuiManager(theme);
}*/
