#include "Core/GuiManager.hpp"

GuiManager::~GuiManager()
{
    delete mGuiManager;
    delete mWm;
    delete mWrapper;
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

void GuiManager::initGui(CL_DisplayWindow &window, const string &theme)
{
    mWm = new CL_GUIWindowManagerTexture(window);
    mGuiManager = new CL_GUIManager(*mWm, theme);

    initGuiWrapper(window.get_geometry().get_width(), 
            window.get_geometry().get_height());
}
