#include "World/Weather/Leaves.hpp"

Leaf::Leaf(float windPower)
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
    k1 = rand() % 30 + 20;
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

void Leaf::update(float windPower, float elapsed)
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

bool Leaf::getRemove()
{
    return remove;
}

Leaves::Leaves(int maxLeaves)
{
    srand(time(NULL));
    mMaxLeaves = maxLeaves;

    mGC = appManager.getGraphic();
}

void Leaves::setLeafLimit(int maxLeaves)
{
    mMaxLeaves = maxLeaves;
}

int Leaves::getLeafLimit()
{
    return mMaxLeaves;
}

void Leaves::update(float _windPower)
{
    float elapsed = appManager.getElapsed() / 1000.0f;

    if (mLeaves.size() < mMaxLeaves)
    {
        if (rand()%200 == 0) mLeaves.push_back(Leaf(_windPower));
    }

    for (int i=0; i < mLeaves.size(); i++)
    {
        mLeaves[i].update(_windPower, elapsed);
        if (mLeaves[i].getRemove())
        {
            mLeaves.erase(mLeaves.begin() + i);
        }
    }
}
