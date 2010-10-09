#include "World/Weather/Moon.hpp"

Moon::Moon(const string &imagePath, float _scaleX, float _scaleY)
{
    string media = utils.getMediaFolder();
    CL_GraphicContext gc = appManager.getGraphic();
    mMoon = CL_Sprite(appManager.getGraphic(), utils.getMediaFolder() + "/" + imagePath);

    mMoonColor = mMoon.get_color();
    mG = 1.0f;
    mB = 1.0f;
    mScaleX = 1.0f;
    mScaleY = 1.0f;

    // Apply image scaling and linear filtrating (to make sprite smooth)
    mMoon.set_scale(_scaleX, _scaleY);
    mMoon.set_linear_filter(true);

    // Find coordinated of the Moon center
    mMoonCenterX = mMoon.get_width()*_scaleX / 2;
    mMoonCenterY = mMoon.get_height()*_scaleY / 2;

    mGlobalTime = 0;
    mRadius = Meters2Pixels(15); // The orbit radius is about 600 pixels
    mAngle = Deg2Rad(-85);       // Start Moon position (85 degrees)
    mColorOffset = 0.0;

    // Center (x;y) of the Moon orbit
    mCenterX = 0;
    mCenterY = appManager.getWindow().get_geometry().get_height();

    mGC = appManager.getGraphic();

    mBlurTexture = CL_Texture(mGC, mGC.get_width(), mGC.get_height());
    mBloomTexture = CL_Texture(mGC, mGC.get_width(), mGC.get_height());
    mSceneTexture = CL_Texture(mGC, mGC.get_width(), mGC.get_height());
    mHueTexture = CL_Texture(mGC, mGC.get_width(), mGC.get_height());

    mBlurBuf = CL_FrameBuffer(mGC);
    mBloomBuf = CL_FrameBuffer(mGC);
    mSceneBuf = CL_FrameBuffer(mGC);
    mHueBuf = CL_FrameBuffer(mGC);

    mBlurBuf.attach_color_buffer(0, mBlurTexture);
    mBloomBuf.attach_color_buffer(0, mBloomTexture);
    mSceneBuf.attach_color_buffer(0, mSceneTexture);
    mHueBuf.attach_color_buffer(0, mHueTexture);

    makeBloomHandle();
    makeBlurHandleH();
    makeBlurHandleV();
    makeHueHandle();

    /*blend_mode.set_blend_function(
        cl_blend_one_minus_src_color, cl_blend_dest_alpha,
        cl_blend_src_color, cl_blend_one_minus_dest_color);*/
    blend_mode.set_blend_function(
            cl_blend_dest_color, cl_blend_one,
            cl_blend_one, cl_blend_one);
    blend_mode.enable_blending(true);
}

void Moon::setScale(float _scaleX, float _scaleY)
{
    mMoon.set_scale(_scaleX, _scaleY);

    mMoonCenterX = mMoon.get_width()*_scaleX / 2;
    mMoonCenterY = mMoon.get_height()*_scaleY / 2;
}

void Moon::update()
{
    float x_local = (mRadius * cos(mAngle)) * 1.3;   // X position of the Moon
    float y_local = (mRadius * sin(mAngle)) * 1.1;   // Y position of the Moon

    float moonX = x_local + mCenterX - mMoonCenterX;
    float moonY = y_local + mCenterY - mMoonCenterY;

    // Draw simple moon
    if (mAngle > Deg2Rad(-30))
    {
        if (mG > 0.85f ) mG -= 0.0002;
        if (mB > 0.75f) mB -= 0.0004;
        if (mScaleX < 1.2) mScaleX += 0.00018;
        if (mScaleY < 1.05) mScaleY += 0.0001;
    }

    mMoon.set_scale(mScaleX, mScaleY);
    mMoon.set_color(CL_Colorf(1.0f, mG, mB));
    mMoon.draw(mGC, moonX, moonY);
    mMoon.set_color(CL_Colorf(1.0f, 1.0f, 1.0f));

    //////// Shaders passes

    // Render moon to own buffer -------------------------
    mGC.set_frame_buffer(mSceneBuf);
    mGC.clear(CL_Colorf::transparent);
    mGC.clear(CL_Colorf::black);
    mMoon.draw(mGC, moonX, moonY);
    mGC.reset_frame_buffer();

    // Bloom pass ---------------------------------------
    mGC.set_frame_buffer(mBloomBuf);
    mGC.set_texture(0, mSceneTexture);
    renderMoon(mProgramBloom);
    mGC.reset_texture(0);
    mGC.reset_frame_buffer();

    // Horizontal Blur pass -----------------------------
    mGC.set_frame_buffer(mBlurBuf);
    mGC.set_texture(0, mBloomTexture);
    renderMoon(mProgramBlurH);
    mGC.reset_texture(0);
    mGC.reset_frame_buffer();

    // Vertical Blur pass -------------------------------
    mGC.set_frame_buffer(mHueBuf);
    mGC.set_texture(0, mBlurTexture);
    renderMoon(mProgramBlurV);
    mGC.reset_texture(0);
    mGC.reset_frame_buffer();

    // Color pass ---------------------------------------
    mGC.set_blend_mode(blend_mode);
    mGC.set_texture(0, mHueTexture);
    renderMoon(mProgramHue, true);
    mGC.reset_texture(0);
    mGC.reset_blend_mode();

    /////// End of shader passes

    mGlobalTime += appManager.getElapsed();

    // Increase angle each 75 ms
    if (mGlobalTime >= 5)
    {
        mGlobalTime = 0;
        if (mAngle <= Deg2Rad(0)) mAngle+=0.0005;
        /*TODO: Do something when moon is hided */
    }
}

void Moon::makeBloomHandle()
{
    mProgramBloom = CL_ProgramObject::load(mGC,
                "media/shaders/bloom/vertext.glsl",
                "media/shaders/bloom/fragment.glsl");

    mProgramBloom.bind_attribute_location(0, "Position");
    mProgramBloom.bind_attribute_location(1, "TextCoord0");

    if (!mProgramBloom.link())
        throw CL_Exception("Unable to link shader!");
}

void Moon::makeBlurHandleH()
{
    mProgramBlurH = CL_ProgramObject::load(mGC,
                "media/shaders/blur/vertext_horizontal.glsl",
                "media/shaders/blur/fragment_horizontal.glsl");

    mProgramBlurH.bind_attribute_location(0, "Position");
    mProgramBlurH.bind_attribute_location(1, "TextCoord0");

    if (!mProgramBlurH.link())
        throw CL_Exception("Unable to link shader!");
}

void Moon::makeBlurHandleV()
{
    mProgramBlurV = CL_ProgramObject::load(mGC,
                "media/shaders/blur/vertext_vertical.glsl",
                "media/shaders/blur/fragment_vertical.glsl");

    mProgramBlurV.bind_attribute_location(0, "Position");
    mProgramBlurV.bind_attribute_location(1, "TextCoord0");

    if (!mProgramBlurV.link())
        throw CL_Exception("Unable to link shader!");
}

void Moon::makeHueHandle()
{
    mProgramHue = CL_ProgramObject::load(mGC,
                "media/shaders/moon-hue/vertext.glsl",
                "media/shaders/moon-hue/fragment.glsl");

    mProgramHue.bind_attribute_location(0, "Position");
    mProgramHue.bind_attribute_location(1, "TextCoord0");

    if (!mProgramHue.link())
        throw CL_Exception("Unable to link hue shader!");
}

void Moon::renderMoon(CL_ProgramObject &program, bool huePass)
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

    mGC.set_program_object(program, cl_program_matrix_modelview_projection);
    mGC.draw_primitives(cl_triangles, 6, primarray);
    mGC.reset_program_object();
}
