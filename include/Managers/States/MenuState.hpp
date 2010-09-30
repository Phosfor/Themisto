#ifndef _MENU_STATE_HPP_
#define _MENU_STATE_HPP_

#include "Managers/StateManager.hpp"
#include "Managers/ApplicationManager.hpp"

#include <boost/foreach.hpp>

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

#define G (9.81*40.0)
#define DropRadius 1

struct drop
{
    private:
        float x, y;
        float x_new, y_new;
        float speed_x, speed_y;

    public:
        drop(float _x = 0, float _wind = 0):
            x(_x), y(0), x_new(0), y_new(0), 
            speed_x(_wind*0.1), speed_y(G*0.1) { }

        void update(float elapsed, float windPower)
        {
            float elapsedSec = elapsed/1000;
            y += speed_y * elapsedSec;
            speed_y += G * elapsedSec;

            x += speed_x * elapsedSec;
            speed_x += windPower * elapsedSec;

            x_new = speed_x * 0.02;
            y_new = speed_y * 0.02;

            CL_Colorf dropColor = CL_Colorf::black;
            dropColor.a = 0.5;

            CL_Draw::line(appManager.getGraphic(), x, y, x-x_new, y-y_new, dropColor);
        }

        float getY() { return y; }
};

class RainManager
{
    private:
        vector<drop> mDrops;
        unsigned int mMaxDrops;
        float mWindPower;

        void processDrops()
        {
            CL_Rect geom = appManager.getWindow().get_geometry();
            int left, right;
            left = right = 0;

            int x1 = geom.get_height() / G * mWindPower;
            if (mWindPower < 0)
            {
                left = 0; 
                right = geom.get_width() - x1;
            }
            else
            {
                left = -x1;
                right = geom.get_width();
            }
            int posX = rand() % (right - left) + left;
            mDrops.push_back(drop(posX, mWindPower));
        }

    public:
        RainManager(int maxDrops = 150, float windPower = 0) : 
            mMaxDrops(maxDrops), mWindPower(windPower) {srand ( time(NULL) );}

        void raining()
        {
            mWindPower = - ((rand()%9)/10 + 6.0) * 50;
            unsigned int counter = 0;
            BOOST_FOREACH(drop &d, mDrops)
            {
                d.update(appManager.getElapsed(), mWindPower);
                if (d.getY() > appManager.getWindow().get_geometry().get_height()-100)
                    mDrops.erase(mDrops.begin()+counter);
                counter++;

            }
            processDrops();
        }
};

class MenuState : public State
{
    CL_Font mStatFont;
    RainManager mRainManager;
    CL_Image mBackground;

    void init() 
    {
        mStatFont = CL_Font(appManager.getGraphic(), "Ubuntu", 30);
        mBackground = CL_Image(appManager.getGraphic(), "media/tree.png");
    }

    void shutdown() {}


    void update() 
    {
        appManager.getGraphic().clear(CL_Colorf::gray);
        mBackground.draw(appManager.getGraphic(), 0, 0);

        mStatFont.draw_text(appManager.getGraphic(), 10, 25, 
                CL_String(cl_format("fps: %1", appManager.getFps())), CL_Colorf::white);
        mStatFont.draw_text(appManager.getGraphic(), 10, 50, 
                CL_String(cl_format("elapsed: %1", floor(appManager.getElapsed()+0.5))), CL_Colorf::white);

        mRainManager.raining();
    }

    string type() { return "MenuState"; }
};

#endif /* _MENU_STATE_HPP_ */
