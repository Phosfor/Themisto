#include "World/Environ/Stars.hpp"

void Stars::setLimit(int limit)
{
   mStars.resize(limit, StarsData(mWindowWidth, mWindowHeight));
   mMaxObjects = limit;
}

StarsData::StarsData(int width, int height)
{
    x = rand() % width;
    y = rand() % height;
    brightness = (float)(rand() % 10 + 1)/10.0f;

    float g = 1.0f - (float)(rand()%4+1)/10.0f;
    float b = 1.0f - Randf(0.5f);

    color = CL_Colorf(1.0f, g, b, brightness);
}

Stars::Stars(int maxStars)
    : EnvironObject(), mBloomSize(0.09f), t1(0), t2(0)
    , mNight(false), mDrawStars(false)
{
    srand(time(NULL));

    mGC = appManager.getGraphic();
    mMaxObjects = maxStars;

    for (int i=0; i < mMaxObjects; i++)
    {
        mStars.push_back(StarsData(mWindowWidth, mWindowHeight));
    }

    mStarsTexture = CL_Texture(mGC, mGC.get_width(), mGC.get_height());
    mStarsBuf = CL_FrameBuffer(mGC);
    mStarsBuf.attach_color_buffer(0, mStarsTexture);

    mBlurTexture = CL_Texture(mGC, mGC.get_width(), mGC.get_height());
    mBlurBuf = CL_FrameBuffer(mGC);
    mBlurBuf.attach_color_buffer(0, mBlurTexture);

    makeBloomHandle();
    makeBlurHandle();

    blend_mode.set_blend_function(
            cl_blend_dest_color, cl_blend_one,
            cl_blend_one, cl_blend_one);
    blend_mode.enable_blending(true);
}

void Stars::update(float windPower, float elapsed, float globalTime)
{
    if (globalTime <= 10.0f) {
        t1 = 0.0f;
    } else if (globalTime >= 13.0f) {
        t1 = 1.0f;
        mNight = true;
    } else {
        mDrawStars = true;
        t1 = ((globalTime - 10.0f) / (13.0f - 10.0f))/5;
        mBloomSize = t1;
    }

    if (mNight)
    {
        if (globalTime <= 21.5f) {
            t2 = 0.0f;
        } else if (globalTime >= 23.8f) {
            t2 = 1.0f;
            mNight = false;
            mDrawStars = false;
        } else {
            t2 = ((globalTime - 10.0f) / (13.0f - 10.0f))/5;
            mBloomSize = 1.0f - t2;
        }
    }

    if (!mDrawStars) return;

    // Draw all the stars to the buffer
    mGC.set_frame_buffer(mStarsBuf);
    mGC.clear(CL_Colorf::transparent);
    //mGC.clear(CL_Colorf::black);
    for (int i=0; i < mMaxObjects; i++)
    {
        // Stars "blinking"
        if (rand() % 60 == 0) continue;
        CL_Draw::circle(mGC, mStars[i].x, mStars[i].y, 1, mStars[i].color);
    }
    mGC.reset_frame_buffer();

    // Process texture with stars using bloom shader
    mGC.set_blend_mode(blend_mode);
        mGC.set_texture(0, mStarsTexture);
            renderStars(mProgramBloom, true);
        mGC.reset_texture(0);
    mGC.reset_blend_mode();

    mGC.set_blend_mode(blend_mode);
        mGC.set_texture(0, mBlurTexture);
            renderStars(mProgramBlur);
        mGC.reset_texture(0);
    mGC.reset_blend_mode();
}

void Stars::makeBloomHandle()
{
    mProgramBloom = CL_ProgramObject::load(mGC,
                "media/shaders/bloom/vertext.glsl",
                "media/shaders/bloom/fragment.glsl");

    mProgramBloom.bind_attribute_location(0, "Position");
    mProgramBloom.bind_attribute_location(1, "TextCoord0");

    if (!mProgramBloom.link())
        throw CL_Exception("Unable to link bloom shader!");
}

void Stars::makeBlurHandle()
{
    mProgramBlur = CL_ProgramObject::load(mGC,
                "media/shaders/blur/vertext_horizontal.glsl",
                "media/shaders/blur/fragment_horizontal.glsl");

    mProgramBlur.bind_attribute_location(0, "Position");
    mProgramBlur.bind_attribute_location(1, "TextCoord0");

    if (!mProgramBlur.link())
        throw CL_Exception("Unable to link blur shader!");
}

void Stars::renderStars(CL_ProgramObject &program, bool bloom)
{
    CL_Rectf rect(0.0f, 0.0f, (float)mGC.get_width(), (float)mGC.get_height());
    CL_Rectf texture_unit1_coords(0.0f, 0.0f, 1.0f, 1.0f);

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

    CL_PrimitivesArray primarray(mGC);
    primarray.set_attributes(0, positions);
    primarray.set_attributes(1, tex1_coords);

    if (bloom) mProgramBloom.set_uniform1f("BloomSize", mBloomSize);

    mGC.set_program_object(program, cl_program_matrix_modelview_projection);
    mGC.draw_primitives(cl_triangles, 6, primarray);
    mGC.reset_program_object();
}
