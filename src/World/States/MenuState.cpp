/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/States/MenuState.hpp"

void MenuState::init()
{
    mGC = appManager().getGraphic();
    mStatFont = new CL_Font(appManager().getGraphic(), "Ubuntu", 30);

    sceneLoader().loadScene("test.xml");
    mEnvironEnabled = environManager().getEnvironEnabled();
}

void MenuState::shutdown() 
{
    delete mStatFont;
}

void MenuState::update()
{
    mGC.clear();
    if (mEnvironEnabled) environManager().update();

    int *time = environManager().getEnvironTime();
    mStatFont->draw_text(appManager().getGraphic(), 10, 25,
            CL_String(cl_format("world time: %1:%2:%3", time[0], time[1], time[2])), CL_Colorf::white);
    mStatFont->draw_text(appManager().getGraphic(), 10, 50,
            CL_String(cl_format("elapsed: %1", int(floor(appManager().getElapsed()+0.5)))), CL_Colorf::white);
    mStatFont->draw_text(appManager().getGraphic(), 10, 75,
            CL_String(cl_format("wind: %1", int(environManager().getWindPower()))), CL_Colorf::white);
}

std::string MenuState::type()
{
    return "MenuState";
}
