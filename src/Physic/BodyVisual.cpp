#include "Physic/BodyVisual.hpp"


BodyVisual::BodyVisual(Body *body)
{

    mBody = body;
}

void BodyVisual::redrawBody()
{
    b2Fixture* fixture = mBody->getb2Body().GetFixtureList();
    CL_Colorf color(0.95f, 0.95f, 0.6f, 1.0f);

    if (fixture != NULL)
    {
        switch (fixture->GetType())
        {
            case b2Shape::e_circle:
                {
                    b2CircleShape *circle = (b2CircleShape*)fixture->GetShape();
                    b2Vec2 center = b2Mul(fixture->GetBody()->GetTransform(), circle->m_p);

                    unsigned int k_segments = 70.0f;
                    float32 k_increment = 2.0f * b2_pi / k_segments;
                    float32 theta = 0.0f;
                    unsigned int radius = circle->m_radius;

                    // Draw solid shape
                    CL_Colorf color2 = CL_Colorf(color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, 0.5);
                    CL_Draw::circle(appManager.getGraphic(), center.x, center.y, radius, color2);

                    // Draw the line from center to the some point of circle
                    CL_Draw::line(appManager.getGraphic(), 
                            center.x, center.y, 
                            radius * cosf(theta) + center.x, radius * sinf(theta) + center.y, 
                            color);

                    // Draw main vertices
                    for (unsigned int i=0; i < k_segments; i++)
                    {
                        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
                        CL_Draw::point(appManager.getGraphic(), v.x, v.y, color);
                        theta += k_increment;
                    }
                }
                break;

            case b2Shape::e_polygon:
                {
                    b2PolygonShape *poly = (b2PolygonShape*)fixture->GetShape();
                    unsigned int vertexCount = poly->m_vertexCount;
                    //b2Assert(vertexCount <= b2_maxPolygonVertices);
                    b2Vec2 vertices[vertexCount];

                    // Get position of vertices in the world
                    for (unsigned int i=0; i < vertexCount; ++i)
                        vertices[i] = b2Mul(fixture->GetBody()->GetTransform(), poly->m_vertices[i]);

                    // Draw fixture using that vertices
                    for (unsigned int j=0; j < vertexCount; ++j)
                        CL_Draw::point(appManager.getGraphic(), vertices[j].x, vertices[j].y, color);
                }
                break;
        }
    }
}
