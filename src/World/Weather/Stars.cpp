#include "World/Weather/Stars.hpp"

Stars::Stars(int maxStars)
{
    srand(time(NULL));

    x = vector<int>(maxStars);
    y = vector<int>(maxStars);
    size = vector<int>(maxStars);
    color = vector<CL_Colorf>(maxStars);

    fill(x.begin(), x.end(), 0);
    fill(y.begin(), y.end(), 0);
    fill(size.begin(), size.end(), 0);
    fill(color.begin(), color.end(), CL_Colorf());

    mGC = appManager.getGraphic();
    mWidth = appManager.getWindow().get_geometry().get_width();
    mHeight = appManager.getWindow().get_geometry().get_height();

    mMaxStars = maxStars;
    for (int i=0; i < mMaxStars; i++)
    {
        x[i] = rand() % mWidth;
        y[i] = rand() % mHeight;
        float brightness = (float)(rand() % 10 + 1)/10.0f;

        float g = 1.0f - (float)(rand()%4+1)/10.0f;
        float b = 1.0f - Randf(0.5f);

        color[i] = CL_Colorf(1.0f, g, b, brightness);
    }

    mWayPattern[0] = CL_Sprite(mGC, "media/milkway/pattern0.png");
    mWayPattern[1] = CL_Sprite(mGC, "media/milkway/pattern1.png");
    mWayPattern[2] = CL_Sprite(mGC, "media/milkway/pattern2.png");
    // Drawing the milkway
    // y = k*x + b
    // k = tg(L)
    // b: y in the intersection with Y
    for (float x=0; x < mWidth/100; x++)
    {
        mMilkWay.push_back(CL_Sprite(mWayPattern[rand()%3]));
        mMilkWay[x].set_alpha(0.5);
        float k = (rand()%15+10)/10.0f;
        mMilkWay[x].set_scale(k, k);
    }

    mSkyPattern = CL_Sprite(mGC, "media/sky_pattern.png");
    for (int i=0; i < 100; i++)
    {
        float x = rand() % mWidth;
        float y = rand() % mHeight;
        x_.push_back(x);
        y_.push_back(y);
    }

    mStarsTexture = CL_Texture(mGC, mGC.get_width(), mGC.get_height());
    mStarsBuf = CL_FrameBuffer(mGC);
    mStarsBuf.attach_color_buffer(0, mStarsTexture);

    mBlurTexture = CL_Texture(mGC, mGC.get_width(), mGC.get_height());
    mBlurBuf = CL_FrameBuffer(mGC);
    mBlurBuf.attach_color_buffer(0, mBlurTexture);

    makeBloomHandle();
    makeBlurHandle();

    mBloomSize = 0.09f;
    t1 = t2 = 0.0f;
    mNight = false;
    mDrawStars = false;

    blend_mode.set_blend_function(
            cl_blend_dest_color, cl_blend_one,
            cl_blend_one, cl_blend_one);
    blend_mode.enable_blending(true);
}

int Stars::getStarsLimit()
{
    return mMaxStars;
}

void Stars::setStarsLimit(int maxStars)
{
    mMaxStars = maxStars;
    /*TODO: Add new stars */
}

void Stars::update(float hours)
{
    if (hours <= 10.0f) {
        t1 = 0.0f;
    } else if (hours >= 13.0f) {
        t1 = 1.0f;
        mNight = true;
    } else {
        mDrawStars = true;
        t1 = ((hours - 10.0f) / (13.0f - 10.0f))/5;
        mBloomSize = t1;
    }

    if (mNight)
    {
        if (hours <= 21.5f) {
            t2 = 0.0f;
        } else if (hours >= 23.8f) {
            t2 = 1.0f;
            mNight = false;
            mDrawStars = false;
        } else {
            t2 = ((hours - 10.0f) / (13.0f - 10.0f))/5;
            mBloomSize = 1.0f - t2;
        }
    }

    if (!mDrawStars) return;

    // Draw all the stars to the buffer
    mGC.set_frame_buffer(mStarsBuf);
    mGC.clear(CL_Colorf::transparent);
    //mGC.clear(CL_Colorf::black);
    for (int i=0; i < mMaxStars; i++)
    {
        // Stars "blinking"
        if (rand() % 60 == 0) continue;
        CL_Draw::circle(mGC, x[i], y[i], 1, color[i]);
    }
    mGC.reset_frame_buffer();

    // Process texture with stars using bloom shader
    mGC.set_blend_mode(blend_mode);
    mGC.set_texture(0, mStarsTexture);
    renderStars(mProgramBloom, true);
    mGC.reset_texture(0);
    mGC.reset_frame_buffer();
    mGC.reset_blend_mode();

    mGC.set_frame_buffer(mBlurBuf);
    mGC.clear(CL_Colorf::transparent);
    //mWayPattern.draw(mGC, 500, 500);
    mGC.reset_frame_buffer();

    mGC.set_blend_mode(blend_mode);
    mGC.set_texture(0, mBlurTexture);
    renderStars(mProgramBlur);
    mGC.reset_texture(0);
    mGC.reset_blend_mode();

    /*for (float x=0; x < mWidth/100; x++)
    {
        float k = tan(Deg2Rad(140));
        float x_ = x * 100;
        float b = mHeight - mHeight*0.4;
        float y = k*x_ + b;
        mMilkWay[x].draw(mGC, x_, y);
    }*/

    /*mSkyPattern.set_color(CL_Colorf(41/255.0f, 87/255.0f, 102/255.0f, 0.1f));
    for (int i=0; i < 100; i++)
    {
        mSkyPattern.draw(mGC, x_[i], y_[i]);
    }*/
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
