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
        float t;

        CL_GraphicContext mGC;
        CL_BlendMode blend_mode;
        CL_ProgramObject mProgramBloom;

        CL_FrameBuffer mStarsBuf;
        CL_Texture mStarsTexture;

        void makeBloomHandle();
        void renderStars(CL_ProgramObject &program);

    public:
        Stars(int maxStars = 200);
        void setStarsLimit(int maxStars);
        int getStarsLimit();
        void update(float hours);
};

#endif /* _WEATHER_STARS_HPP_ */
