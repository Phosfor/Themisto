#include "Core/SceneLoader.hpp"


SceneLoader::SceneLoader(){}

void SceneLoader::loadScene()
{

    //add earth
    b2BodyDef earthDef;
    earthDef.position.Set(100.0f, 500.0f);
    b2Body* earth = physicManager.getWorld().CreateBody(&earthDef);

    b2PolygonShape earthShape;
    earthShape.SetAsBox(800.0f, 2.0f);

    earth->CreateFixture(&earthShape, 0.0f);

    Body *earthBody = new Body(earth);
    BodyVisual *earthVisual = new BodyVisual(earthBody);
    earthBody->setVisual(earthVisual);
    physicManager.registerBody(earthBody);

    //add box
    b2BodyDef box1Def;
    box1Def.position.Set(100.0f, 10.0f);
    box1Def.type = b2_dynamicBody;
    b2Body* box1 = physicManager.getWorld().CreateBody(&box1Def);

    b2PolygonShape box1Shape;
    box1Shape.SetAsBox(10.0f, 10.0f);

    b2FixtureDef box1Fixture;
    box1Fixture.shape = &box1Shape;
    box1Fixture.density = 100.0f;
    box1Fixture.friction = 0.3;
    box1->CreateFixture(&box1Fixture);

    Body *box1Body = new Body(box1);
    box1Body->setVisual(new BodyVisual(box1Body));
    physicManager.registerBody(box1Body);

     //add some circle
    b2BodyDef circle1Def;
    circle1Def.position.Set(150.0f, 7.0f);
    circle1Def.type = b2_dynamicBody;
    b2Body* circle1 = physicManager.getWorld().CreateBody(&circle1Def);

    b2CircleShape circle1Shape;
    circle1Shape.m_radius = 20.0f;
    circle1Shape.m_p = circle1Def.position;

    b2FixtureDef circle1Fixture;
    circle1Fixture.shape = &circle1Shape;
    circle1Fixture.density = 200.0f;
    circle1Fixture.friction = 0.8f;
    circle1->CreateFixture(&circle1Fixture);

    Body *circle1Body = new Body(circle1);
    circle1Body->setVisual(new BodyVisual(circle1Body));
    physicManager.registerBody(circle1Body);

    //add polygon
    /*b2BodyDef polygon1Def;
    polygon1Def.position.Set(200.0f, 7.0f);
    polygon1Def.type = b2_dynamicBody;
    b2Body* polygon1 = physicManager.getWorld().CreateBody(&polygon1Def);

    b2PolygonShape polygon1Shape;
    int vertexCount = 6;
    b2Vec2 vertices[vertexCount];
    for(int i=0; i<vertexCount; i++)
    {
        vertices[i] = b2Vec2((float)i, (float)(i+1));
    }
    polygon1Shape.Set(vertices, vertexCount);

    b2FixtureDef polygon1Fixture;
    polygon1Fixture.shape = &polygon1Shape;
    polygon1Fixture.density = 150.0f;
    polygon1Fixture.friction = 0.1;
    polygon1->CreateFixture(&polygon1Fixture);

    Body *polygon1Body = new Body(polygon1);
    polygon1Body->setVisual(new BodyVisual(polygon1Body));
    physicManager.registerBody(polygon1Body);*/
}
