#include "Core/SceneLoader.hpp"
#include "Core/PhysicManager.hpp"

SceneLoader::SceneLoader(){}

void SceneLoader::loadScene()
{
    //add earth
    b2BodyDef earthDef;
    earthDef.position.Set(100.0f, 200.0f);
    b2Body* earth = physicManager.getWorld().CreateBody(&earthDef);

    b2PolygonShape earthShape;
    earthShape.SetAsBox(100.0f, 2.0f);

    earth->CreateFixture(&earthShape, 0.0f);

    Body *earthBody = new Body(earth);
    physicManager.registerBody(earthBody);

    //add box
    b2BodyDef box1Def;
    box1Def.position.Set(100.0f, 10.0f);
    box1Def.type = b2_dynamicBody;
    b2Body* box1 = physicManager.getWorld().CreateBody(&box1Def);

    b2PolygonShape box1Shape;
    box1Shape.SetAsBox(2.0f, 2.0f);

    b2FixtureDef box1Fixture;
    box1Fixture.shape = &box1Shape;
    box1Fixture.density = 100.0f;
    box1Fixture.friction = 0.3;
    box1->CreateFixture(&box1Fixture);

    Body *box1Body = new Body(box1);
    physicManager.registerBody(box1Body);
}
