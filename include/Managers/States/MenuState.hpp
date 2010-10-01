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
#define Pi 3.14159265
#define Rad2Deg(rad) rad * 180 / Pi
#define Deg2Rad(deg) deg * Pi/180

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

            x_new = speed_x * 0.025;
            y_new = speed_y * 0.025;

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
            if (mDrops.size() > mMaxDrops) return;

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

    // Moon
    CL_Sprite mMoon;
    int mMoonCenterX, mMoonCenterY;

    float mGlobTime;
    float mAngle;

    // Orbit ellipse
    int mRadius;
    float mCenterX, mCenterY;

    // Fractals
    vector< vector<CL_Vec2f> > mPoints;
    int mPointsCounter;

    vector<CL_Vec2f> mQueue;
    int mMaxLevel;
    int mCurLevel;
    CL_Vec2f a,b;

    CL_Vec2f mStart, mEnd;

    void init() 
    {
        mStatFont = CL_Font(appManager.getGraphic(), "Ubuntu", 30);
        mBackground = CL_Image(appManager.getGraphic(), "media/tree.png");

        // Moon
        mMoon = CL_Sprite(appManager.getGraphic(), "media/moon.png");
        mMoon.set_scale(0.2, 0.2);
        mMoon.set_linear_filter(true);
        mMoonCenterX = mMoon.get_width()*0.2 / 2;
        mMoonCenterY = mMoon.get_height()*0.2 / 2;

        mGlobTime = 0;
        mAngle = Deg2Rad(-85);
        mRadius = 600;

        mCenterX = 0;
        mCenterY = appManager.getWindow().get_geometry().get_height();

        // ---------- Lighting
        /*a = CL_Vec2f(200, 50);
        b = CL_Vec2f(150, 250);
        mMaxLevel = 4;
        mCurLevel = 0;
        mPointsCounter = 0;

        mQueue.push_back(a);

        while (!mQueue.empty())
        {
            mPoints.push_back(vector<CL_Vec2f>());
            mStart = mQueue.back();
            mQueue.pop_back();
            mEnd = CL_Vec2f(rand() % appManager.getWindow().get_geometry().get_width(),  450);
            processFact(mStart, mEnd, 0);
            mPointsCounter++;
        }*/
    }

    void processFact(CL_Vec2f &a, CL_Vec2f &b, int level)
    {
        if ((b-a).length() < 20)
        {
            if (rand()%((int)(30/(mEnd-mStart).length())) == 0/* && mCurLevel <= mMaxLevel */)
            {
                mQueue.push_back(a);
                mCurLevel++;
            }

            mPoints[mPointsCounter].push_back(a);
            return;
        }

        CL_Vec2f o = CL_Vec2f( (a.x+b.x)/2, (a.y+b.y)/2 );
        CL_Vec2f oX = CL_Vec2f(o.y, -o.x);
        oX.normalize();
        oX *= (float)rand()/(float)RAND_MAX - 0.5;
        oX *= (a-b).length();
        CL_Vec2f N = o + oX;

        processFact(a, N, level+1);
        processFact(N, b, level+1);

        if (level == 0) mPoints[mPointsCounter].push_back(b);
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


        float x_local = (mRadius * cos(mAngle)) * 1.3;
        float y_local = (mRadius * sin(mAngle)) * 1.1;

        mMoon.draw(appManager.getGraphic(), x_local + mCenterX - mMoonCenterX, y_local + mCenterY - mMoonCenterY);
        mRainManager.raining();

        mGlobTime += appManager.getElapsed();

        if (mGlobTime >= 200)
        {
            mGlobTime = 0;
            if (mAngle <= Deg2Rad(0)) mAngle+=0.0005;
            else mAngle = Deg2Rad(-90);
        }

        /*for (int j=0; j < mPoints.size(); j++)
        {
            for (int i=0; i < mPoints[j].size()-1; i++)
            {
                CL_Draw::circle(appManager.getGraphic(), mPoints[j][i].x, mPoints[j][i].y, 2, CL_Colorf::black);
                CL_Draw::line(appManager.getGraphic(), mPoints[j][i].x, mPoints[j][i].y,
                        mPoints[j][i+1].x, mPoints[j][i+1].y, CL_Colorf::red);
            }
        }*/
    }

    string type() { return "MenuState"; }
};

#endif /* _MENU_STATE_HPP_ */
