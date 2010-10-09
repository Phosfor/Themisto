#include "World/Weather/Stars.hpp"

Stars::~Stars()
{
    delete mBloomSlider;
}

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

    mStarsTexture = CL_Texture(mGC, mGC.get_width(), mGC.get_height());
    mStarsBuf = CL_FrameBuffer(mGC);
    mStarsBuf.attach_color_buffer(0, mStarsTexture);

    makeBloomHandle();
    mBloomSize = 0.0f;
    mTimer = 0.0f;
    mIncrease = true;

    blend_mode.set_blend_function(
            cl_blend_dest_color, cl_blend_one,
            cl_blend_one, cl_blend_one);
    blend_mode.enable_blending(true);

    // TODO: REMOVE
    mBloomSlider = new CL_Slider(&guiManager.getWrapper());
    mBloomSlider->set_horizontal(true);
    mBloomSlider->set_position(200);
    mBloomSlider->set_ranges(100, 400, 5, 5);
    mBloomSlider->set_geometry(CL_RectPS(mWidth - 180 - 5, 100, 180, 30));
    mBloomSlider->func_value_changed().set(this, &Stars::bloomChanged);
}

void Stars::bloomChanged()
{
    mBloomSize = mBloomSlider->get_position() / 1000.0f;
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

void Stars::update()
{
    // Draw all the stars to the buffer
    mGC.set_frame_buffer(mStarsBuf);
    mGC.clear(CL_Colorf::transparent);
    mGC.clear(CL_Colorf::black);
    for (unsigned int i=0; i < mMaxStars; i++)
    {
        //CL_Draw::point(mGC, x[i], y[i], color[i]);
        CL_Draw::circle(mGC, x[i], y[i], 1.5, color[i]);
    }
    mGC.reset_frame_buffer();

    // Process texture with stars using bloom shader
    mGC.set_blend_mode(blend_mode);
    mGC.set_texture(0, mStarsTexture);
    renderStars(mProgramBloom);
    mGC.reset_texture(0);
    mGC.reset_blend_mode();

    mTimer += appManager.getElapsed();
    if (mTimer >= 75 && mIncrease)
    {
        mBloomSize += 0.001f;
        mTimer = 0;
        if (mBloomSize >= 0.20f) mIncrease = false;
    }
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

void Stars::renderStars(CL_ProgramObject &program)
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

    mProgramBloom.set_uniform1f("BloomSize", mBloomSize);

    mGC.set_program_object(program, cl_program_matrix_modelview_projection);
    mGC.draw_primitives(cl_triangles, 6, primarray);
    mGC.reset_program_object();
}
