/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "World/Objects/Grass.hpp"

Grass::Grass(CL_Pointf position)
{
    mPosition = position;
    mType = GrassObject;
    mGC = appManager().getGraphic();
    mImageHandle = CL_Sprite(mGC, "media/plants/3.png");

    mTexture1 = CL_Texture(mGC, mImageHandle.get_width(), mImageHandle.get_height());
    //mTexture1 = CL_Texture(mGC, mGC.get_width(), mGC.get_height());
    mBuf1 = CL_FrameBuffer(mGC);
    mBuf1.attach_color_buffer(0, mTexture1);

    mAmplitudeFactor = 53.0f;
    mYStretch = 40.0f;
    mTime = 0.0f;

    mShader = CL_ProgramObject::load(mGC, "media/shaders/grass/v.glsl", "media/shaders/grass/f.glsl");
    mShader.bind_attribute_location(0, "Position");
    mShader.bind_attribute_location(1, "TexCoord0");
    if(!mShader.link())
        std::cout << "Unable to link shader in Grass class!\n";
}

void Grass::drawTexture(const CL_Rectf &rect, const CL_Rectf &texture_unit1_coords)
{
    CL_Vec2f positions[6] =
    {
        CL_Vec2f(rect.left, rect.top),
        CL_Vec2f(rect.right, rect.top),
        CL_Vec2f(rect.left, rect.bottom),
        CL_Vec2f(rect.right, rect.top),
        CL_Vec2f(rect.left, rect.bottom),
        CL_Vec2f(rect.right, rect.bottom)
    };

    CL_Vec2f tex1_coords[6] =
    {
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
        CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
        CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
    };

    mShader.set_uniform1f("AmplitudeFactor", mAmplitudeFactor);
    mShader.set_uniform1f("YStretch", mYStretch);
    mShader.set_uniform1f("Time", mTime);

    CL_PrimitivesArray prim_array(mGC);
    prim_array.set_attributes(0, positions);
    prim_array.set_attributes(1, tex1_coords);
    mGC.draw_primitives(cl_triangles, 6, prim_array);
}

void Grass::update(float elapsed)
{
    mTime += appManager().getElapsed() / 20.0f;

    mGC.set_frame_buffer(mBuf1);
    mGC.clear(CL_Colorf::transparent);
    mImageHandle.draw(mGC, 0, 0);
    //mImageHandle.draw(mGC, 400, 200);
    mGC.reset_frame_buffer();

    mGC.set_texture(0, mTexture1);
    mGC.set_program_object(mShader, cl_program_matrix_modelview_projection);
    drawTexture(CL_Rectf(200, 687, CL_Sizef(mImageHandle.get_width(), mImageHandle.get_height())),
               CL_Rectf(0, 0, 1.0f, 1.0f));
    /*drawTexture(CL_Rectf(0, 0, mGC.get_width(), mGC.get_height()),
               CL_Rectf(0, 0, 1.0f, 1.0f));*/
    mGC.reset_texture(0);
}
