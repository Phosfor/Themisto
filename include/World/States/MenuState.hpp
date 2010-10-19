#ifndef _MENU_STATE_HPP_
#define _MENU_STATE_HPP_

#include "Core/StateManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "World/WorldManager.hpp"

#include <boost/foreach.hpp>

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

using namespace std;

class MenuState : public State
{
    private:
    CL_Font *mStatFont;
    CL_Image mBackground;
    CL_Image mFg1;

    CL_CheckBox *mRainState;
    CL_PushButton *mSome;
    CL_Slider *mWindSlider;
    CL_Slider *mDropSlider;

    CL_Label *mWindLabel;

    CL_GraphicContext mGC;
    CL_Rect mGeom;

    void rainStateChanged();
    void windPowerChanged();
    void dropNumChanged();

    void init();
    void shutdown();
    void update();

    string type();
};

#endif /* _MENU_STATE_HPP_ */
