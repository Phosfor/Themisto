#ifndef _MENU_STATE_HPP_
#define _MENU_STATE_HPP_

#include "Core/StateManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/SceneLoader.hpp"

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

using namespace std;

class MenuState : public State
{
    private:
    CL_Font *mStatFont;
    bool mEnvironEnabled;

    CL_GraphicContext mGC;

    void init();
    void shutdown();
    void update();

    string type();
};

#endif /* _MENU_STATE_HPP_ */
