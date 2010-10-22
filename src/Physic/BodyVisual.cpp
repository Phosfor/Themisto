#include "Physic/BodyVisual.hpp"


BodyVisual::BodyVisual(Body *body)
{

    mBody = body;
}

void BodyVisual::redrawBody()
{
    b2Fixture* fixture = mBody->getb2Body().GetFixtureList();
    if (fixture != NULL)
    {
        switch (fixture->GetType())
        {
            case b2Shape::e_circle:
                b2CircleShape *circle = (b2CircleShape*)fixture->GetShape();

                b2Vec2 center = b2Mul(fixture->GetBody()->GetTransform(), circle->m_p);
                CL_Draw::circle(appManager.getGraphic(), center.x, center.y, circle->m_radius, CL_Colorf::black);
                break;
        }
    }

    CL_Draw::circle(appManager.getGraphic() , mBody->getb2Body().GetWorldCenter().x,
                                         mBody->getb2Body().GetWorldCenter().y, 5.0f, CL_Colorf::red);
}
