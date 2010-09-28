#ifndef _MENU_STATE_HPP_
#define _MENU_STATE_HPP_

#include "Managers/StateManager.hpp"

class MenuState : public State
{
    void init() 
    {
        /*TODO: Make statistic manager */
        /*TRACK_VALUE_FLOAT("fps", &GameApplication::getFPS, &gameApplication);
        TRACK_VALUE_INT("batches", &Ogre::RenderWindow::getBatchCount, graphicSystem.getWindow());
        TRACK_VALUE_INT("triangles", &Ogre::RenderWindow::getTriangleCount, graphicSystem.getWindow());*/
    }

    void shutdown() 
    {
    }

        // Change game state here ...
//        READY_TO_ADVANCE();

    void update() {}
    string type() { return "MenuState"; }
};

#endif /* _MENU_STATE_HPP_ */
