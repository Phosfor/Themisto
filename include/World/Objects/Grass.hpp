#ifndef _ENVIRON_GRASS_HPP_
#define _ENVIRON_GRASS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/GuiManager.hpp"

class Grass
{
    private:
        CL_Sprite mImageHandle;
        CL_GraphicContext mGC;

        CL_Texture mTexture1;
        CL_FrameBuffer mBuf1;
        CL_ProgramObject mShader;

        float mAmplitudeFactor, mYStretch, mTime;

        void drawTexture(const CL_Rectf &rect, const CL_Rectf &texture_unit1_coords);

    public:
        float getAmp() { return mAmplitudeFactor; }
        float getStretcH() { return mYStretch; }
        void setAmp(float amp) { mAmplitudeFactor = amp; }
        void setStretch(float amp) { mYStretch = amp; }

        Grass();
        void update();
};

#endif /* _ENVIRON_GRASS_HPP_ */
