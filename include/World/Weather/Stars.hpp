#ifndef _WEATHER_STARS_HPP_
#define _WEATHER_STARS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/GuiManager.hpp"

class Stars
{
    private:
        vector<int> x, y, size;
        vector<CL_Colorf> color;

        int mMaxStars;
        int mWidth, mHeight;
        float mBloomSize;
        float t1, t2;
        bool mNight;
        bool mDrawStars;

        CL_Sprite mWayPattern[3]; // Milkway texture patterns
        vector<CL_Sprite> mMilkWay;
        CL_Sprite mSkyPattern;
        vector<int> x_,y_;

        CL_GraphicContext mGC;
        CL_BlendMode blend_mode;

        CL_ProgramObject mProgramBloom;
        CL_ProgramObject mProgramBlur;

        CL_FrameBuffer mStarsBuf;
        CL_Texture mStarsTexture;

        CL_FrameBuffer mBlurBuf;
        CL_Texture mBlurTexture;

        void makeBloomHandle();
        void makeBlurHandle();

        void renderStars(CL_ProgramObject &program, bool bloom = false);

    public:
        Stars(int maxStars = 200);
        void setStarsLimit(int maxStars);
        int getStarsLimit();
        void update(float hours);
};

#endif /* _WEATHER_STARS_HPP_ */
