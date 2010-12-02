/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _ENVIRON_GRASS_HPP_
#define _ENVIRON_GRASS_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"
#include "Core/GuiManager.hpp"

#include "World/Objects/Object.hpp"

class Grass : public Object
{
    private:
        CL_Pointf mPosition;
        CL_Sprite mImageHandle;
        CL_GraphicContext mGC;

        CL_Texture mTexture1;
        CL_FrameBuffer mBuf1;
        CL_ProgramObject mShader;

        float mAmplitudeFactor, mYStretch, mTime;

        void drawTexture(const CL_Rectf &rect, const CL_Rectf &texture_unit1_coords);

    public:
        Grass(CL_Pointf position);
    
        float getAmp() { return mAmplitudeFactor; }
        float getStretcH() { return mYStretch; }
        void setAmp(float amp) { mAmplitudeFactor = amp; }
        void setStretch(float amp) { mYStretch = amp; }
        
        CL_Pointf getPosition() { return mPosition; }
        void setPosition(CL_Pointf point) { mPosition = point; }
        
        Grass();
        void update(float elapsed);
};

#endif /* _ENVIRON_GRASS_HPP_ */
