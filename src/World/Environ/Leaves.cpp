#include "World/Environ/Leaves.hpp"

void Leaves::processLeaves(CL_GraphicContext &gc, float windPower, int i)
{
    mLeaves[i].timer = mLeaves[i].addedAngle = 0;
    mLeaves[i].x_speed = mLeaves[i].y_speed = 0;

    mLeaves[i].animation = false;

    // Where to create new leaf (refers to the wind blowing direction)
    if (windPower < 0)
        mLeaves[i].x = mWindowWidth;
    else
        mLeaves[i].x = 0;

    // The height where new leaf will be located
    mLeaves[i].y = mWindowHeight - mWindowHeight*((rand()%6+2)/10.0);

    // Some magic numbers for the leaf trajectory
    mLeaves[i].k1 = rand() % 30 + 20;
    mLeaves[i].k2 = (rand() % 7 + 3) / 10.0;

    // Load some random leaf surface
    mLeaves[i].leafType = rand()%4;
    mLeaves[i].imageHandle = CL_Sprite(gc, cl_format("media/leaves/%1.png", mLeaves[i].leafType));

    mLeaves[i].timeout = rand() % 300;

    // Rotate leaf surface at some random angle
    int angle = rand() % 360 + 1;
    mLeaves[i].imageHandle.set_angle(CL_Angle::from_degrees(angle));

    mLeaves[i].speed_koef = rand()%45 + 50;
}

Leaves::Leaves(int maxLeaves)
    : EnvironObject(), firstTime(true)
{
    srand(time(NULL));
    mGC = appManager.getGraphic();
    mMaxObjects = maxLeaves;
}

void Leaves::update(float windPower, float elapsed, float globalTime)
{
    // Init pack part of leaves
    if (firstTime)
    {
        for (int i=0; i < mMaxObjects; i++)
        {
            mLeaves.push_back(LeafData());
            processLeaves(mGC, windPower, i);
        }
        firstTime = false;
    }

    for (int i=0; i < mMaxObjects; i++)
    {
        if (mLeaves[i].timeout > 0)
        {
            mLeaves[i].timeout--;
        }
        else
        {
            mLeaves[i].x += mLeaves[i].x_speed * elapsed;
            mLeaves[i].y += mLeaves[i].y_speed * elapsed;

            float y = mLeaves[i].k1 * sinf(mLeaves[i].k2 * Deg2Rad(mLeaves[i].x)) + mLeaves[i].y;

            if (floor(abs(y - mLeaves[i].y)+0.5) >= mLeaves[i].k1) mLeaves[i].animation = true;

            if (mLeaves[i].animation)
            {
                mLeaves[i].timer += elapsed * 1000.0;
                if (mLeaves[i].timer >= 100)
                {
                    int angle = rand() % 15 + 5;
                    mLeaves[i].addedAngle += angle;

                    mLeaves[i].imageHandle.rotate_pitch(CL_Angle::from_degrees(angle));

                    mLeaves[i].timer = 0;

                    if (mLeaves[i].addedAngle >= 180)
                    {
                        mLeaves[i].animation = false;
                        mLeaves[i].addedAngle = 0;
                    }
                }
            }

            if (windPower > 0) {
                if (mLeaves[i].x > mWindowWidth + 10 || mLeaves[i].y > mWindowHeight) 
                    processLeaves(mGC, windPower, i);
            } else {
                if (mLeaves[i].x < -mLeaves[i].imageHandle.get_width() || mLeaves[i].y > mWindowHeight)
                    processLeaves(mGC, windPower, i);
            }

            mLeaves[i].x_speed = mLeaves[i].speed_koef * windPower * elapsed;
            mLeaves[i].y_speed = G * elapsed;

            mLeaves[i].imageHandle.draw(mGC, mLeaves[i].x, y);
        }
    }
}
