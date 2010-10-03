#ifndef _GUI_MANAGER_HPP_
#define _GUI_MANAGER_HPP_

#include <ClanLib/gui.h>
#include <ClanLib/core.h>
#include <vector>
#include <boost/serialization/singleton.hpp>

using namespace std;

#define guiManager (GuiManager::get_mutable_instance())
#define guiManagerConst (GuiManager::get_const_instance())

/*class ComplicatedWindow : public CL_Window
{
public:
    ComplicatedWindow(CL_GUIManager *manager, const CL_DisplayWindowDescription &desc)
    : CL_Window(manager, desc)
    {
        func_render().set(this, &GameWindow::on_render);
        set_constant_repaint(true);
    }
 
private:    
    void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
    {
        update_game_logic();
        render_game(gc);
    }
 
    CL_PushButton *button;
};*/

class GuiManager : public boost::serialization::singleton<GuiManager>
{
    private:
        CL_GUIManager *mGuiManager;
        string mThemeName;

    public:
        //~GuiManager();
        CL_GUIManager *getHandle();
        string getThemeName();
        void setThemeName(const string &name);

        //void initGui(const string &theme);
};

#endif /* _GUI_MANAGER_HPP_ */
