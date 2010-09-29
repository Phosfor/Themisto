#ifndef _GUI_MANAGER_HPP_
#define _GUI_MANAGER_HPP_

#include <boost/serialization/singleton.hpp>
#include <boost/foreach.hpp>
#include <ClanLib/gui.h>
#include <vector>

using namespace std;

#define guiManager (GuiManager::get_mutable_instance())
#define guiManagerConst (GuiManager::get_const_instance())

class GuiManager : public boost::serialization::singleton<GuiManager>
{
    private:
        CL_GUIManager *mGuiManager;
        string mThemeName;

    private:
        CL_GUIManager *getHandle();
        string getThemeName();
        void setThemeName(const string &name);
};

#endif /* _GUI_MANAGER_HPP_ */
