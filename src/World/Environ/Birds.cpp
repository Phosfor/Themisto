#include "World/Environ/Birds.hpp"

Birds::Birds()
{
    mGC = appManager.getGraphic();
    mWidth = mGC.get_width();
    mHeight = mGC.get_height();

    for (int i=1; i <= 10; i++)
    {
        // Load all animation frames
        mImageDesc.add_frame(CL_ImageProviderFactory::load(cl_format("media/bird/%1.png", i)));
    }
    mBirdImage = CL_Sprite(mGC, mImageDesc);
    mBirdImage.set_scale(0.5, 0.5);
    mBirdImage.set_linear_filter(true);

    mProbability = 800;
    mFlying = false;
}

void Birds::update()
{
    if (!mFlying && rand()%mProbability == 0) 
    {
        cout << "Bird is added\n";
        mFlying = true;

        x_speed = rand() %30 + 150;
        if (rand()%2 == 0) {
            x = 0 - mBirdImage.get_width();
            mRightSide = 1; // The speed should be less than zero (to make flying left)
            mBirdImage.set_angle_yaw(CL_Angle::from_degrees(180));
        } else {
            x = mWidth;
            mRightSide = -1;
            mBirdImage.set_angle_yaw(CL_Angle::from_degrees(0));
        }

        y = mHeight * (float)(rand()%3 + 2) / 10.0;
    }

    if (!mFlying) return;

    x += mRightSide * x_speed * appManager.getElapsed() / 1000.0f;
    mBirdImage.update();
    mBirdImage.draw(mGC, x, y);

    if (mRightSide == 1 && x >= mWidth) 
        mFlying = false;
    else if (mRightSide == -1 && x <= 0 - mBirdImage.get_width())
        mFlying = false;
}
