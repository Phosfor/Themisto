#ifndef _MENU_STATE_HPP_
#define _MENU_STATE_HPP_

#include "Core/StateManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/EnvironManager.hpp"

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

#include "World/Objects/Grass.hpp"

using namespace std;

class MenuState : public State
{
    private:
    CL_Font *mStatFont;
    CL_Image mBackground;

    CL_GraphicContext mGC;

    // Will be moved to some manager
    Grass mGrass;

    void init();
    void shutdown();
    void update();

    string type();
};

#endif /* _MENU_STATE_HPP_ */
