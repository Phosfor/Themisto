#ifndef _ENVIRON_BIRDS_HPP_
#define _ENVIRON_BIRDS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class Birds
{
    private:
        float x, y, x_speed;
        int mWidth;
        int mHeight;
        int mProbability; // The probability that bird will begin to fly current frame
        bool mFlying; // Some bird is already flying in the sky
        float mRightSide; // The bird is created at the right side of the screen

        CL_SpriteDescription mImageDesc;
        CL_Sprite mBirdImage;
        CL_GraphicContext mGC;

    public:
        Birds();
        void update();
};

#endif /* _ENVIRON_BIRDS_HPP_ */
