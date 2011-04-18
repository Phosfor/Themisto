#include <boost/python.hpp>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/Joints/b2DistanceJoint.h>
#include <Box2D/Dynamics/Joints/b2FrictionJoint.h>
#include <Box2D/Dynamics/Joints/b2GearJoint.h>
#include <Box2D/Dynamics/Joints/b2Joint.h>
#include <Box2D/Dynamics/Joints/b2LineJoint.h>
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
#include <Box2D/Dynamics/Joints/b2PrismaticJoint.h>
#include <Box2D/Dynamics/Joints/b2PulleyJoint.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/Joints/b2WeldJoint.h>

#include "Scripts/Physic/Convert.hpp"
#include "Scripts/Physic/b2World.hpp"

bp::list get_bodies(b2World *world) {
    bp::list bodies;
    for (b2Body *body = world->GetBodybp::list();
         body;
         body = body->GetNext())
    {
        bodies.append(convert_ptr(body));
    }
    return bodies;
}

bp::list get_joints(b2World *world) {
    bp::list joints;
    for (b2Joint *joint = world->GetJointbp::list();
         joint;
         joint = joint->GetNext())
    {
        joints.append(convert_ptr(joint));
    }
    return joints;
}

b2Body *create_body_1(b2World *world,
                      b2BodyType type,
                      b2Vec2 position,
                      float32 angle,
                      b2Vec2 linear_velocity,
                      float32 angular_velocity,
                      float32 linear_damping,
                      float32 angular_damping,
                      bool allow_sleep,
                      bool awake,
                      bool fixed_rotation,
                      bool bullet,
                      bool active,
                      b2UserData user_data,
                      float32 inertia_scale)
{
    b2BodyDef body_def;
    body_def.type = type;
    body_def.position = position;
    body_def.angle = angle;
    body_def.linearVelocity = linear_velocity;
    body_def.angularVelocity = angular_velocity;
    body_def.linearDamping = linear_damping;
    body_def.angularDamping = angular_damping;
    body_def.allowSleep = allow_sleep;
    body_def.awake = awake;
    body_def.fixedRotation = fixed_rotation;
    body_def.bullet = bullet;
    body_def.active = active;
    body_def.userData = user_data;
    body_def.inertiaScale = inertia_scale;
    return world->CreateBody(&body_def);
}

template <b2BodyType T>
b2Body *create_body_2(b2World *world,
                      b2Vec2 position,
                      float32 angle,
                      b2Vec2 linear_velocity,
                      float32 angular_velocity,
                      float32 linear_damping,
                      float32 angular_damping,
                      bool allow_sleep,
                      bool awake,
                      bool fixed_rotation,
                      bool bullet,
                      bool active,
                      b2UserData user_data,
                      float32 inertia_scale)
{
    b2BodyDef body_def;
    body_def.type = T;
    body_def.position = position;
    body_def.angle = angle;
    body_def.linearVelocity = linear_velocity;
    body_def.angularVelocity = angular_velocity;
    body_def.linearDamping = linear_damping;
    body_def.angularDamping = angular_damping;
    body_def.allowSleep = allow_sleep;
    body_def.awake = awake;
    body_def.fixedRotation = fixed_rotation;
    body_def.bullet = bullet;
    body_def.active = active;
    body_def.userData = user_data;
    body_def.inertiaScale = inertia_scale;
    return world->CreateBody(&body_def);
}

b2Joint *create_revolute_joint(b2World *world,
                               b2Body *body_a,
                               b2Body *body_b,
                               b2Vec2 anchor,
                               bool limit_enabled,
                               float32 lower_angle,
                               float32 upper_angle,
                               bool motor_enabled,
                               float32 motor_speed,
                               float32 max_motor_torque,
                               b2UserData user_data,
                               bool collide_connected)
{
    b2RevoluteJointDef revolute_joint_def;
    revolute_joint_def.Initialize(body_a, body_b, anchor);
    revolute_joint_def.enableLimit = limit_enabled;
    revolute_joint_def.lowerAngle = lower_angle;
    revolute_joint_def.upperAngle = upper_angle;
    revolute_joint_def.enableMotor = motor_enabled;
    revolute_joint_def.motorSpeed = motor_speed;
    revolute_joint_def.maxMotorTorque = max_motor_torque;
    revolute_joint_def.userData = user_data;
    revolute_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&revolute_joint_def);
}

b2Joint *create_prismatic_joint(b2World *world,
                                b2Body *body_a,
                                b2Body *body_b,
                                b2Vec2 anchor,
                                b2Vec2 axis,
                                bool limit_enabled,
                                float32 lower_translation,
                                float32 upper_translation,
                                bool motor_enabled,
                                float32 max_motor_force,
                                float32 motor_speed,
                                b2UserData user_data,
                                bool collide_connected)
{
    b2PrismaticJointDef prismatic_joint_def;
    prismatic_joint_def.Initialize(body_a, body_b, anchor, axis);
    prismatic_joint_def.enableLimit = limit_enabled;
    prismatic_joint_def.lowerTranslation = lower_translation;
    prismatic_joint_def.upperTranslation = upper_translation;
    prismatic_joint_def.enableMotor = motor_enabled;
    prismatic_joint_def.maxMotorForce = max_motor_force;
    prismatic_joint_def.motorSpeed = motor_speed;
    prismatic_joint_def.userData = user_data;
    prismatic_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&prismatic_joint_def);
}

b2Joint *create_distance_joint(b2World *world,
                               b2Body *body_a,
                               b2Body *body_b,
                               b2Vec2 anchor_a,
                               b2Vec2 anchor_b,
                               float32 frequency,
                               float32 damping_ratio,
                               b2UserData user_data,
                               bool collide_connected)
{
    b2DistanceJointDef distance_joint_def;
    distance_joint_def.Initialize(body_a, body_b, anchor_a, anchor_b);
    distance_joint_def.frequencyHz = frequency;
    distance_joint_def.dampingRatio = damping_ratio;
    distance_joint_def.userData = user_data;
    distance_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&distance_joint_def);
}

b2Joint *create_pulley_joint(b2World *world,
                             b2Body *body_a,
                             b2Body *body_b,
                             b2Vec2 ground_anchor_a,
                             b2Vec2 ground_anchor_b,
                             b2Vec2 anchor_a,
                             b2Vec2 anchor_b,
                             float32 ratio,
                             b2UserData user_data,
                             bool collide_connected)
{
    b2PulleyJointDef pulley_joint_def;
    pulley_joint_def.Initialize(body_a, body_b, ground_anchor_a,
                                ground_anchor_b, anchor_a, anchor_b,
                                ratio);
    pulley_joint_def.userData = user_data;
    pulley_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&pulley_joint_def);
}

b2Joint *create_mouse_joint(b2World *world,
                            b2Body *body,
                            b2Vec2 target,
                            float32 max_force,
                            float32 frequency,
                            float32 damping_ratio,
                            b2UserData user_data,
                            bool collide_connected)
{
    b2MouseJointDef mouse_joint_def;
    mouse_joint_def.bodyB = body;
    mouse_joint_def.target = target;
    mouse_joint_def.maxForce = max_force;
    mouse_joint_def.frequencyHz = frequency;
    mouse_joint_def.dampingRatio = damping_ratio;
    mouse_joint_def.userData = user_data;
    mouse_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&mouse_joint_def);
}

b2Joint *create_gear_joint(b2World *world,
                           b2Joint *joint_a,
                           b2Joint *joint_b,
                           float32 ratio,
                           b2UserData user_data,
                           bool collide_connected)
{
    b2GearJointDef gear_joint_def;
    gear_joint_def.joint1 = joint_a;
    gear_joint_def.joint2 = joint_b;
    gear_joint_def.ratio = ratio;
    gear_joint_def.userData = user_data;
    gear_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&gear_joint_def);
}

b2Joint *create_line_joint(b2World *world,
                           b2Body *body_a,
                           b2Body *body_b,
                           b2Vec2 anchor,
                           b2Vec2 axis,
                           bool limit_enabled,
                           float32 lower_translation,
                           float32 upper_translation,
                           bool motor_enabled,
                           float32 max_motor_force,
                           float32 motor_speed,
                           b2UserData user_data,
                           bool collide_connected)
{
    b2LineJointDef line_joint_def;
    line_joint_def.Initialize(body_a, body_b, anchor, axis);
    line_joint_def.enableLimit = limit_enabled;
    line_joint_def.lowerTranslation = lower_translation;
    line_joint_def.upperTranslation = upper_translation;
    line_joint_def.enableMotor = motor_enabled;
    line_joint_def.maxMotorForce = max_motor_force;
    line_joint_def.motorSpeed = motor_speed;
    line_joint_def.userData = user_data;
    line_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&line_joint_def);
}

b2Joint *create_weld_joint(b2World *world,
                           b2Body *body_a,
                           b2Body *body_b,
                           b2Vec2 anchor,
                           b2UserData user_data,
                           bool collide_connected)
{
    b2WeldJointDef weld_joint_def;
    weld_joint_def.Initialize(body_a, body_b, anchor);
    weld_joint_def.userData = user_data;
    weld_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&weld_joint_def);
}

b2Joint *create_friction_joint(b2World *world,
                               b2Body *body_a,
                               b2Body *body_b,
                               b2Vec2 anchor,
                               float32 max_force,
                               float32 max_torque,
                               b2UserData user_data,
                               bool collide_connected)
{
    b2FrictionJointDef friction_joint_def;
    friction_joint_def.Initialize(body_a, body_b, anchor);
    friction_joint_def.maxForce = max_force;
    friction_joint_def.maxTorque = max_torque;
    friction_joint_def.userData = user_data;
    friction_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&friction_joint_def);
}

struct QueryCallbackWrapper1 : b2QueryCallback {
    object callback_;

    explicit QueryCallbackWrapper1(object callback)
    : callback_(callback)
    { }

    bool ReportFixture(b2Fixture *fixture) {
        return callback_(fixture);
    }
};

void query_aabb_1(b2World *world,
                  object callback,
                  b2Vec2 lower_bound,
                  b2Vec2 upper_bound)
{
    QueryCallbackWrapper1 callback_wrapper(callback);
    b2AABB aabb;
    aabb.lowerBound = lower_bound;
    aabb.upperBound = upper_bound;
    world->QueryAABB(&callback_wrapper, aabb);
}

struct QueryCallbackWrapper2 : b2QueryCallback {
    bp::list result_;

    bool ReportFixture(b2Fixture *fixture) {
        result_.append(convert_ptr(fixture));
        return true;
    }
};

bp::list query_aabb_2(b2World *world,
                  b2Vec2 lower_bound,
                  b2Vec2 upper_bound)
{
    QueryCallbackWrapper2 callback_wrapper;
    b2AABB aabb;
    aabb.lowerBound = lower_bound;
    aabb.upperBound = upper_bound;
    world->QueryAABB(&callback_wrapper, aabb);
    return callback_wrapper.result_;
}

void wrap_world() {
    class_<b2World>("World", init<b2Vec2, bool>())
        .add_property("destruction_bp::listener", object(), &b2World::SetDestructionbp::listener)
        .add_property("contact_filter", object(), &b2World::SetContactFilter)
        .add_property("contact_bp::listener", object(), &b2World::SetContactbp::listener)
        .add_property("debug_draw", object(), &b2World::SetDebugDraw)
        .add_property("bodies", &get_bodies)
        .add_property("joints", &get_joints)
        .add_property("contact_bp::list", make_function(&b2World::GetContactbp::list, bp::return_internal_reference<>()))
        .add_property("warm_starting", object(), &b2World::SetWarmStarting)
        .add_property("continuous_physics", object(), &b2World::SetContinuousPhysics)
        .add_property("sub_stepping", object(), &b2World::SetSubStepping)
        .add_property("proxy_count", &b2World::GetProxyCount)
        .add_property("body_count", &b2World::GetBodyCount)
        .add_property("joint_count", &b2World::GetJointCount)
        .add_property("contact_count", &b2World::GetContactCount)
        .add_property("gravity", &b2World::GetGravity, &b2World::SetGravity)
        .add_property("locked", &b2World::IsLocked)
        .add_property("auto_clear_forces", &b2World::GetAutoClearForces, &b2World::SetAutoClearForces)
        .add_property("contact_manager", make_function(&b2World::GetContactManager, bp::return_internal_reference<>()))

        .def("create_body", &create_body_1, bp::return_internal_reference<>(),
             (arg("self"),
              arg("type"),
              arg("position")=b2Vec2(0.0f, 0.0f),
              arg("angle")=0.0f,
              arg("linear_velocity")=b2Vec2(0.0f, 0.0f),
              arg("angular_velocity")=0.0f,
              arg("linear_damping")=0.0f,
              arg("angular_damping")=0.0f,
              arg("allow_sleep")=true,
              arg("awake")=true,
              arg("fixed_rotation")=false,
              arg("bullet")=false,
              arg("active")=true,
              arg("user_data")=object(),
              arg("inertia_scale")=1.0f))
        .def("create_static_body", &create_body_2<b2_staticBody>, bp::return_internal_reference<>(),
             (arg("self"),
              arg("position")=b2Vec2(0.0f, 0.0f),
              arg("angle")=0.0f,
              arg("linear_velocity")=b2Vec2(0.0f, 0.0f),
              arg("angular_velocity")=0.0f,
              arg("linear_damping")=0.0f,
              arg("angular_damping")=0.0f,
              arg("allow_sleep")=true,
              arg("awake")=true,
              arg("fixed_rotation")=false,
              arg("bullet")=false,
              arg("active")=true,
              arg("user_data")=object(),
              arg("inertia_scale")=1.0f))
        .def("create_kinematic_body", &create_body_2<b2_kinematicBody>, bp::return_internal_reference<>(),
             (arg("self"),
              arg("position")=b2Vec2(0.0f, 0.0f),
              arg("angle")=0.0f,
              arg("linear_velocity")=b2Vec2(0.0f, 0.0f),
              arg("angular_velocity")=0.0f,
              arg("linear_damping")=0.0f,
              arg("angular_damping")=0.0f,
              arg("allow_sleep")=true,
              arg("awake")=true,
              arg("fixed_rotation")=false,
              arg("bullet")=false,
              arg("active")=true,
              arg("user_data")=object(),
              arg("inertia_scale")=1.0f))
        .def("create_dynamic_body", &create_body_2<b2_dynamicBody>, bp::return_internal_reference<>(),
             (arg("self"),
              arg("position")=b2Vec2(0.0f, 0.0f),
              arg("angle")=0.0f,
              arg("linear_velocity")=b2Vec2(0.0f, 0.0f),
              arg("angular_velocity")=0.0f,
              arg("linear_damping")=0.0f,
              arg("angular_damping")=0.0f,
              arg("allow_sleep")=true,
              arg("awake")=true,
              arg("fixed_rotation")=false,
              arg("bullet")=false,
              arg("active")=true,
              arg("user_data")=object(),
              arg("inertia_scale")=1.0f))
        .def("destroy_body", &b2World::DestroyBody)
        .def("create_revolute_joint", &create_revolute_joint, bp::return_internal_reference<>(),
             (arg("self"),
              arg("body_a"),
              arg("body_b"),
              arg("anchor"),
              arg("limit_enabled")=false,
              arg("lower_angle")=0.0f,
              arg("upper_angle")=0.0f,
              arg("motor_enabled")=false,
              arg("motor_speed")=0.0f,
              arg("max_motor_torque")=0.0f,
              arg("user_data")=object(),
              arg("collide_connected")=false))
        .def("create_prismatic_joint", &create_prismatic_joint, bp::return_internal_reference<>(),
             (arg("self"),
              arg("body_a"),
              arg("body_b"),
              arg("anchor"),
              arg("axis"),
              arg("limit_enabled")=false,
              arg("lower_translation")=0.0f,
              arg("upper_translation")=0.0f,
              arg("motor_enabled")=false,
              arg("max_motor_force")=0.0f,
              arg("motor_speed")=0.0f,
              arg("user_data")=object(),
              arg("collide_connected")=false))
        .def("create_distance_joint", &create_distance_joint, bp::return_internal_reference<>(),
             (arg("self"),
              arg("body_a"),
              arg("body_b"),
              arg("anchor_a"),
              arg("anchor_b"),
              arg("frequency")=0.0f,
              arg("damping_ratio")=0.0f,
              arg("user_data")=object(),
              arg("collide_connected")=false))
        .def("create_pulley_joint", &create_pulley_joint, bp::return_internal_reference<>(),
             (arg("self"),
              arg("body_a"),
              arg("body_b"),
              arg("ground_anchor_a"),
              arg("ground_anchor_b"),
              arg("anchor_a"),
              arg("anchor_b"),
              arg("ratio")=0.0f,
              arg("user_data")=object(),
              arg("collide_connected")=false))
        .def("create_mouse_joint", &create_mouse_joint, bp::return_internal_reference<>(),
             (arg("self"),
              arg("body"),
              arg("target"),
              arg("max_force")=0.0f,
              arg("frequency")=5.0f,
              arg("damping_ratio")=0.7f,
              arg("user_data")=object(),
              arg("collide_connected")=false))
        .def("create_gear_joint", &create_gear_joint, bp::return_internal_reference<>(),
             (arg("self"),
              arg("joint_a"),
              arg("joint_b"),
              arg("ratio"),
              arg("user_data")=object(),
              arg("collide_connected")=false))
        .def("create_line_joint", &create_line_joint, bp::return_internal_reference<>(),
             (arg("self"),
              arg("body_a"),
              arg("body_b"),
              arg("anchor"),
              arg("axis"),
              arg("limit_enabled")=false,
              arg("lower_translation")=0.0f,
              arg("upper_translation")=0.0f,
              arg("motor_enabled")=false,
              arg("max_motor_force")=0.0f,
              arg("motor_speed")=0.0f,
              arg("user_data")=object(),
              arg("collide_connected")=false))
        .def("create_weld_joint", &create_weld_joint, bp::return_internal_reference<>(),
             (arg("self"),
              arg("body_a"),
              arg("body_b"),
              arg("anchor"),
              arg("user_data")=object(),
              arg("collide_connected")=false))
        .def("create_friction_joint", &create_friction_joint, bp::return_internal_reference<>(),
             (arg("self"),
              arg("body_a"),
              arg("body_b"),
              arg("anchor"),
              arg("max_force")=0.0f,
              arg("max_force")=0.0f,
              arg("user_data")=object(),
              arg("collide_connected")=false))
        .def("destroy_joint", &b2World::DestroyJoint)
        .def("step", &b2World::Step)
        .def("clear_forces", &b2World::ClearForces)
        .def("draw_debug_data", &b2World::DrawDebugData)
        .def("query_aabb", &query_aabb_1)
        .def("query_aabb", &query_aabb_2)
        .def("ray_cast", &b2World::RayCast)
    ;
}
