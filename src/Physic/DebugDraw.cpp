#include "Physic/DebugDraw.hpp"

DebugDraw::DebugDraw()
{
    mGC = appManager.getGraphic();
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    CL_Colorf cl_color(color.r, color.g, color.b);
    for (unsigned short int i=0; i < vertexCount - 1; ++i)
        CL_Draw::line(mGC, 
                vertices[i].x, vertices[i].y,
                vertices[i+1].x, vertices[i+1].y,
                cl_color);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    CL_Colorf cl_color(color.r, color.g, color.b);

    for (unsigned short int i=0; i < vertexCount - 1; ++i)
        CL_Draw::line(mGC, 
                vertices[i].x, vertices[i].y,
                vertices[i+1].x, vertices[i+1].y,
                cl_color);

    std::vector<CL_Vec2f> triangles;
    for (unsigned short int i = 0; i < vertexCount; ++i)
      triangles.push_back(CL_Vec2f(vertices[i].x, vertices[i].y));

    if (!triangles.empty())
    {
      CL_PrimitivesArray primarray(mGC);
      primarray.set_attributes(0, &triangles[0]);
      primarray.set_attribute(1, cl_color);
      mGC.set_program_object(cl_program_color_only);
      mGC.draw_primitives(cl_triangle_fan, triangles.size(), primarray);
      mGC.reset_program_object();
    }
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    CL_Colorf cl_color(color.r, color.g, color.b);

    unsigned int k_segments = 70.0f;
    float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;

    for (unsigned int i=0; i < k_segments; i++)
    {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        CL_Draw::point(appManager.getGraphic(), v.x, v.y, cl_color);
        theta += k_increment;
    }
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    CL_Colorf cl_color(color.r, color.g, color.b);
    CL_Colorf color2 = CL_Colorf(cl_color.r * 0.5f, cl_color.g * 0.5f, cl_color.b * 0.5f, 0.5);

    unsigned int k_segments = 70.0f;
    float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;

    // Draw solid shape
    CL_Draw::circle(appManager.getGraphic(), center.x, center.y, radius, color2);

    // Draw the border
    for (unsigned int i=0; i < k_segments; i++)
    {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        CL_Draw::point(appManager.getGraphic(), v.x, v.y, cl_color);
        theta += k_increment;
    }

    // Draw the line from center to the border
    b2Vec2 p = center + radius * axis;
    CL_Draw::line(mGC, center.x, center.y, p.x, p.y, cl_color);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) { }
void DebugDraw::DrawTransform(const b2Transform& xf) { }

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
    CL_Colorf cl_color(color.r, color.g, color.b);

    CL_Draw::point(mGC, p.x, p.y, cl_color);
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color) { }
