#include "World/Weather/Leaves.hpp"

Leaves::Leaves(int maxLeaves)
{
    srand(time(NULL));
    mMaxLeaves = maxLeaves;

    mGC = appManager.getGraphic();
    mWidth = mGC.get_width();
    mHeight = mGC.get_height();
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
