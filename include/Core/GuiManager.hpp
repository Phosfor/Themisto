/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _GUI_MANAGER_HPP_
#define _GUI_MANAGER_HPP_

#include <ClanLib/gui.h>
#include <ClanLib/core.h>
#include <vector>
#include <boost/serialization/singleton.hpp>

using namespace std;

#define guiManager (GuiManager::get_mutable_instance())
#define guiManagerConst (GuiManager::get_const_instance())

class GuiManager : public boost::serialization::singleton<GuiManager>
{
    private:
        CL_GUIManager *mGuiManager;
        CL_GUIWindowManagerTexture *mWm;
        CL_GUIComponent *mWrapper;

        string mThemeName;

        void initGuiWrapper(float width, float height);

    public:
        ~GuiManager();

        CL_GUIManager getHandle();
        CL_GUIWindowManagerTexture &getWM();
        CL_GUIComponent &getWrapper();

        string getThemeName();
        void setThemeName(const string &name);

        void initGui(CL_DisplayWindow &window, const string &theme);
};

#endif /* _GUI_MANAGER_HPP_ */
