#include "Physic/BodyVisual.hpp"


BodyVisual::BodyVisual(Body *body)
{

    mBody = body;
}

void BodyVisual::redrawBody()
{

      /*
        b2Fixture* fixture = mBody->GetFixtureList;
        if(fixture != NULL)
        {
        
                b2PolygonShape *shape =   (b2PolygonShape*)fixture.GetShape();
                int vertexCount = shape->GetVertexCount();
                if(vertexCount > 0)
                {
                    b2Vec2 vertex1 = shape->GetVertex(0);
                    for(int i=1; i<shape->GetVertexCount(); i++)
                    {
                        b2Vec2 vertex2 = shape->GetVertex(i);
                        CL_Draw::line(appManager.getGraphic(), vertex1.x,
                                         vertex1.y, vertex2.x, vertex2.y, CL_Colorf::red);
                        vertex1 = vertex2;

                    }
                }
                

        }
        */
        
    CL_Draw::circle(appManager.getGraphic() , mBody->getb2Body().GetWorldCenter().x,
                                         mBody->getb2Body().GetWorldCenter().y, 5.0f, CL_Colorf::red);
}
