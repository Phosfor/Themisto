#include "Core/SceneLoader.hpp"


SceneLoader::SceneLoader(){}

void SceneLoader::loadScene()
{

    //add earth
    b2BodyDef earthDef;
    earthDef.position.Set(100.0f, 300.0f);
    b2Body* earth = physicManager.getWorld().CreateBody(&earthDef);

    b2PolygonShape earthShape;
    earthShape.SetAsBox(800.0f, 2.0f);

    earth->CreateFixture(&earthShape, 0.0f);

    Body *earthBody = new Body(earth);
    BodyVisual *earthVisual = new BodyVisual;
    earthBody->setVisual(earthVisual);
    physicManager.registerBody(earthBody);

    //add box
    b2BodyDef box1Def;
    box1Def.position.Set(100.0f, 50.0f);
    box1Def.type = b2_dynamicBody;
    b2Body* box1 = physicManager.getWorld().CreateBody(&box1Def);

    b2PolygonShape box1Shape;
    box1Shape.SetAsBox(10.0f, 10.0f);

    b2FixtureDef box1Fixture;
    box1Fixture.shape = &box1Shape;
    box1Fixture.density = 100.0f;
    box1Fixture.friction = 0.3;


    Body *box1Body = new Body(box1);
    box1Body->setVisual(new BodyVisual);
    box1Body->createFixture(&box1Fixture);
    physicManager.registerBody(box1Body);

     //add some circle
    b2BodyDef circle1Def;

    circle1Def.position.Set(110.0f, 7.0f);
    circle1Def.type = b2_dynamicBody;
    b2Body* circle1 = physicManager.getWorld().CreateBody(&circle1Def);
    circle1->SetBullet(true);
    b2MassData mass;
    mass.mass = 10;
    mass.center.Set(0.0f, 0.0f);
    mass.I = 100;
    circle1->SetMassData(&mass);

    b2CircleShape circle1Shape;
    circle1Shape.m_radius = 20.0f;
    circle1Shape.m_p.Set(0.0f, 10.0f);

    b2FixtureDef circle1Fixture;
    circle1Fixture.shape = &circle1Shape;
    circle1Fixture.density = 200.0f;
    circle1Fixture.friction = 0.8f;


    Body *circle1Body = new Body(circle1);
    circle1Body->setVisual(new BodyVisual);
    circle1Body->createFixture(&circle1Fixture);
    circle1->ResetMassData();
    physicManager.registerBody(circle1Body);

    //add polygon
    b2BodyDef polygon1Def;
    polygon1Def.position.Set(200.0f, 7.0f);
    polygon1Def.type = b2_dynamicBody;
    b2Body* polygon1 = physicManager.getWorld().CreateBody(&polygon1Def);

    b2PolygonShape polygon1Shape;
    int vertexCount = 6;
    b2Vec2 vertices[vertexCount];

    vertices[0] = b2Vec2(0.0f, 0.0f);
    vertices[1] = b2Vec2(5.0f, 0.0f);
    vertices[2] = b2Vec2(30.0f, 5.0f);
    vertices[3] = b2Vec2(5.0f, 30.0f);
    vertices[4] = b2Vec2(0.0f, 5.0f);
    vertices[5] = b2Vec2(1.0f, 1.0f);

    polygon1Shape.Set(vertices, vertexCount);

    b2FixtureDef polygon1Fixture;
    polygon1Fixture.shape= &polygon1Shape;
    polygon1Fixture.density = 150.0f;
    polygon1Fixture.friction = 0.1;


    Body *polygon1Body = new Body(polygon1);
    polygon1Body->setVisual(new BodyVisual);
    polygon1Body->createFixture(&polygon1Fixture);
    physicManager.registerBody(polygon1Body);

    //define a 'U' shaped area of square boxes
    float MPP = 0.2;
	for (int i=1; i<=15; i++)
	{
		b2BodyDef bd;
		bd.type = b2_dynamicBody;

		if (i <= 5)
			bd.position.Set(500*MPP, (400+i*32)*MPP);
		else if (i <= 10)
			bd.position.Set((505 + (i-5)*32)*MPP, 500*MPP);
		else
			bd.position.Set(702*MPP, (400+(i-10)*32)*MPP);
		b2Body *b = physicManager.getWorld().CreateBody(&bd);
		b2PolygonShape box;
		box.SetAsBox(16*MPP, 16*MPP);
		b2FixtureDef fd;
		fd.shape = &box;
		fd.density = 1.0f;
		fd.friction = 0.3f;
		fd.restitution = 0.5f;
		b->CreateFixture(&fd);
		Body* bb = new Body(b);
		BodyVisual* v = new BodyVisual;
		bb->setVisual(v);
		physicManager.registerBody(bb);
	}
}
