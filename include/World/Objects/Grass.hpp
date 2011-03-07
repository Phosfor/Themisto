/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

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
