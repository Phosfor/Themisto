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

#ifndef _GUI_MANAGER_HPP_
#define _GUI_MANAGER_HPP_

#include <vector>
#include <string>

#include <ClanLib/gui.h>
#include <ClanLib/core.h>
#include <boost/serialization/singleton.hpp>

class GuiManager : public boost::serialization::singleton<GuiManager>
{
    private:
        CL_GUIManager *mGuiManager;
        CL_GUIWindowManagerTexture *mWm;
        CL_GUIComponent *mWrapper;

        std::string mThemeName;

        void initGuiWrapper(float width, float height);

    public:
        ~GuiManager();

        CL_GUIManager &getHandle();
        CL_GUIWindowManagerTexture &getWM();
        CL_GUIComponent &getWrapper();

        std::string getThemeName();
        void setThemeName(const std::string &name);

        void initGui(CL_DisplayWindow &window, const std::string &theme);
};

inline GuiManager &guiManager() { return GuiManager::get_mutable_instance(); }

#endif /* _GUI_MANAGER_HPP_ */
