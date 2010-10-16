#ifndef _WEATHER_LEAVES_HPP_
#define _WEATHER_LEAVES_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class Leaf
{
    private:
        float x, y_offset, x_speed, k1, k2;
        int leafType;
        bool remove;
        int width, height;
        CL_Sprite imageHandle;
        CL_GraphicContext mGC;

        bool animation;
        float addedAngle;
        float timer;

    public:
        Leaf(float windPower)
        {
            mGC = appManager.getGraphic();
            width = appManager.getGraphic().get_width();
            height = appManager.getGraphic().get_height();

            // Where to create new leaf (refers to the wind blowing direction)
            if (windPower < 0)
                x = width;
            else
                x = 0;

            // The height where new leaf will be located
            y_offset = height - height*((rand()%6+2)/10.0);

            x_speed = 0.1;

            // Used for leaf pitching animation
            addedAngle = 0;
            timer = 0.0;

            // Some magic numbers for the leaf trajectory
            k1 = rand() % 25 + 15;
            k2 = (rand() % 7 + 3) / 10.0;

            // Load some random leaf surface
            leafType = rand()%3+1;
            imageHandle = CL_Sprite(mGC, cl_format("media/leaves/%1.png", leafType));

            // Rotate leaf surface at some random angle
            int angle = rand() % 360 + 1;
            imageHandle.set_angle(CL_Angle::from_degrees(angle));

            // Toggled true when the leaf is out of the screen
            remove = false;

            // Toggled when the leaf reached min/max trajectory points
            animation = false;
        }

        void update(float windPower, float elapsed)
        {
            x += x_speed * elapsed;
            float y = k1 * sinf(k2 * Deg2Rad(x)) + y_offset;

            if (floor(abs(y - y_offset)+0.5) >= k1) animation = true;

            if (animation)
            {
                timer += elapsed * 1000.0;
                if (timer >= 100)
                {

                    int angle = rand() % 15 + 5;
                    addedAngle += angle;

                    imageHandle.rotate_pitch(CL_Angle::from_degrees(angle));

                    timer = 0;

                    if (addedAngle >= 180)
                    {
                        animation = false;
                        addedAngle = 0;
                    }
                }
            }

            if (windPower > 0) {
                if (x > width + 10 || y > height) remove = true;
            } else {
                if (x < 10 || y > height) remove = true;
            }

            // TODO: * rand koef (instead of 50)
            x_speed = windPower * 70.0 * elapsed;

            imageHandle.draw(mGC, x, y);
        }

        bool getRemove()
        {
            return remove;
        }
};

class Leaves
{
    private:
        vector<float> x, y, x_speed, y_speed;
        vector<int> leaf_type, timeout;
        CL_Sprite mLeafHandles[4];
        bool mFirstTime;
        vector<Leaf> mLeaves;

        int mMaxLeaves;
        float mWidth, mHeight;

        CL_GraphicContext mGC;

    public:
        Leaves(int maxLeaves = 5);
        void setLeafLimit(int maxLeaves);
        int getLeafLimit();
        void setProbability(int prob);
        void addLeaf(float _windPower, int i);
        void update(float _windPower);
};

#endif /* _WEATHER_LEAVES_HPP_ */
