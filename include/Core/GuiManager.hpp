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
        string mThemeName;

    public:
        ~GuiManager();
        CL_GUIManager getHandle();

        string getThemeName();
        void setThemeName(const string &name);

        void initGui(CL_GUIWindowManagerTexture &wm, const string &theme);
};

#endif /* _GUI_MANAGER_HPP_ */
