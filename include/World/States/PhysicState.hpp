#ifndef _PHYSIC_STATE_HPP_
#define _PHYSIC_STATE_HPP_

#include "Core/StateManager.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/PhysicManager.hpp"
#include "Core/SceneLoader.hpp"

#include <boost/foreach.hpp>

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

using namespace std;

class PhysicState : public State
{
    private:
    CL_Font *mStatFont;
    CL_Image mBackground;

    CL_GraphicContext mGC;

    void init();
    void shutdown();
    void update();

    string type();
};

#endif /* _PHYSIC_STATE_HPP_ */
