#pragma once

#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/Joints/b2Joint.h>

bp::list get_fixtures(b2Body *body)
{
    bp::list fixtures;
    for (b2Fixture *fixture = body->GetFixtureList();
         fixture;
         fixture = fixture->GetNext())
    {
        fixtures.append(convert_ptr(fixture));
    }
    return fixtures;
}

bp::list get_joints(b2Body *body)
{
    bp::list joints;
    for (b2JointEdge *joint_edge = body->GetJointList();
         joint_edge;
         joint_edge = joint_edge->next)
    {
        joints.append(convert_ptr(joint_edge->joint));
    }
    return joints;
}

b2Fixture *create_fixture(b2Body *body, b2Shape *shape,
                          void *user_data,
                          float32 friction, float32 restitution, float32 density,
                          bool sensor,
                          uint16 category_bits, uint16 mask_bits, uint16 group_index)
{
    b2FixtureDef fixture_def;
    fixture_def.shape = shape;
    fixture_def.userData = user_data;
    fixture_def.friction = friction;
    fixture_def.restitution = restitution;
    fixture_def.density = density;
    fixture_def.isSensor = sensor;
    fixture_def.filter.categoryBits = category_bits;
    fixture_def.filter.maskBits = mask_bits;
    fixture_def.filter.groupIndex = group_index;

    return body->CreateFixture(&fixture_def);
}

b2Fixture *create_circle_fixture(b2Body *body,
                                 b2Vec2 position, float32 radius,
                                 void *user_data,
                                 float32 friction, float32 restitution, float32 density,
                                 bool sensor,
                                 uint16 category_bits, uint16 mask_bits, uint16 group_index)
{
    b2CircleShape circle_shape;
    circle_shape.m_p = position;
    circle_shape.m_radius = radius;

    b2FixtureDef fixture_def;
    fixture_def.shape = &circle_shape;
    fixture_def.userData = user_data;
    fixture_def.friction = friction;
    fixture_def.restitution = restitution;
    fixture_def.density = density;
    fixture_def.isSensor = sensor;
    fixture_def.filter.categoryBits = category_bits;
    fixture_def.filter.maskBits = mask_bits;
    fixture_def.filter.groupIndex = group_index;

    return body->CreateFixture(&fixture_def);
}

void set_polygon_shape_vertices(b2PolygonShape *polygon_shape, bp::list vertices)
{
    b2Vec2 arr[b2_maxPolygonVertices];
    long n = len(vertices);
    for (long i = 0; i != n; ++i) {
        arr[i] = bp::extract<b2Vec2>(vertices[i]);
    }
    polygon_shape->Set(arr, n);
}

b2Fixture *create_polygon_fixture(b2Body *body, bp::list vertices,
                                  void *user_data,
                                  float32 friction, float32 restitution, float32 density,
                                  bool sensor,
                                  uint16 category_bits, uint16 mask_bits, uint16 group_index)
{
    b2PolygonShape polygon_shape;
    if (len(vertices)) {
        set_polygon_shape_vertices(&polygon_shape, vertices);
    } else {
        polygon_shape.SetAsBox(0.5f, 0.5f);
    }

    b2FixtureDef fixture_def;
    fixture_def.shape = &polygon_shape;
    fixture_def.userData = user_data;
    fixture_def.friction = friction;
    fixture_def.restitution = restitution;
    fixture_def.density = density;
    fixture_def.isSensor = sensor;
    fixture_def.filter.categoryBits = category_bits;
    fixture_def.filter.maskBits = mask_bits;
    fixture_def.filter.groupIndex = group_index;

    return body->CreateFixture(&fixture_def);
}

b2Fixture *create_box_fixture(b2Body *body, float32 half_width, float32 half_height,
                              b2Vec2 center, float32 angle,
                              void *user_data,
                              float32 friction, float32 restitution, float32 density,
                              bool sensor,
                              uint16 category_bits, uint16 mask_bits, uint16 group_index)
{
    b2PolygonShape polygon_shape;
    polygon_shape.SetAsBox(half_width, half_height, center, angle);

    b2FixtureDef fixture_def;
    fixture_def.shape = &polygon_shape;
    fixture_def.userData = user_data;
    fixture_def.friction = friction;
    fixture_def.restitution = restitution;
    fixture_def.density = density;
    fixture_def.isSensor = sensor;
    fixture_def.filter.categoryBits = category_bits;
    fixture_def.filter.maskBits = mask_bits;
    fixture_def.filter.groupIndex = group_index;

    return body->CreateFixture(&fixture_def);
}

void set_mass_data(b2Body *body, float32 mass, b2Vec2 local_center, float32 inertia)
{
    b2MassData mass_data;
    mass_data.mass = mass;
    mass_data.center = local_center;
    mass_data.I = inertia;
    body->SetMassData(&mass_data);
}
