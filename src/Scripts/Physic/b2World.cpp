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

boost::python::list get_bodies(b2World *world) {
    boost::python::list bodies;
    for (b2Body *body = world->GetBodyList();
         body;
         body = body->GetNext())
    {
        bodies.append(convert_ptr(body));
    }
    return bodies;
}

boost::python::list get_joints(b2World *world) {
    boost::python::list joints;
    for (b2Joint *joint = world->GetJointList();
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
    //body_def.userData = user_data;
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
    //body_def.userData = user_data;
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
    //revolute_joint_def.userData = user_data;
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
    //prismatic_joint_def.userData = user_data;
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
                               bool collide_connected)
{
    b2DistanceJointDef distance_joint_def;
    distance_joint_def.Initialize(body_a, body_b, anchor_a, anchor_b);
    distance_joint_def.frequencyHz = frequency;
    distance_joint_def.dampingRatio = damping_ratio;
    //distance_joint_def.userData = user_data;
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
                             bool collide_connected)
{
    b2PulleyJointDef pulley_joint_def;
    pulley_joint_def.Initialize(body_a, body_b, ground_anchor_a,
                                ground_anchor_b, anchor_a, anchor_b,
                                ratio);
    //pulley_joint_def.userData = user_data;
    pulley_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&pulley_joint_def);
}

b2Joint *create_mouse_joint(b2World *world,
                            b2Body *body,
                            b2Vec2 target,
                            float32 max_force,
                            float32 frequency,
                            float32 damping_ratio,
                            bool collide_connected)
{
    b2MouseJointDef mouse_joint_def;
    mouse_joint_def.bodyB = body;
    mouse_joint_def.target = target;
    mouse_joint_def.maxForce = max_force;
    mouse_joint_def.frequencyHz = frequency;
    mouse_joint_def.dampingRatio = damping_ratio;
    //mouse_joint_def.userData = user_data;
    mouse_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&mouse_joint_def);
}

b2Joint *create_gear_joint(b2World *world,
                           b2Joint *joint_a,
                           b2Joint *joint_b,
                           float32 ratio,
                           bool collide_connected)
{
    b2GearJointDef gear_joint_def;
    gear_joint_def.joint1 = joint_a;
    gear_joint_def.joint2 = joint_b;
    gear_joint_def.ratio = ratio;
    //gear_joint_def.userData = user_data;
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
    //line_joint_def.userData = user_data;
    line_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&line_joint_def);
}

b2Joint *create_weld_joint(b2World *world,
                           b2Body *body_a,
                           b2Body *body_b,
                           b2Vec2 anchor,
                           bool collide_connected)
{
    b2WeldJointDef weld_joint_def;
    weld_joint_def.Initialize(body_a, body_b, anchor);
    //weld_joint_def.userData = user_data;
    weld_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&weld_joint_def);
}

b2Joint *create_friction_joint(b2World *world,
                               b2Body *body_a,
                               b2Body *body_b,
                               b2Vec2 anchor,
                               float32 max_force,
                               float32 max_torque,
                               bool collide_connected)
{
    b2FrictionJointDef friction_joint_def;
    friction_joint_def.Initialize(body_a, body_b, anchor);
    friction_joint_def.maxForce = max_force;
    friction_joint_def.maxTorque = max_torque;
    //friction_joint_def.userData = user_data;
    friction_joint_def.collideConnected = collide_connected;
    return world->CreateJoint(&friction_joint_def);
}

struct QueryCallbackWrapper1 : b2QueryCallback {
    boost::python::object callback_;

    explicit QueryCallbackWrapper1(boost::python::object callback)
    : callback_(callback)
    { }

    bool ReportFixture(b2Fixture *fixture) {
        return callback_(fixture);
    }
};

void query_aabb_1(b2World *world,
                  boost::python::object callback,
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
    boost::python::list result_;

    bool ReportFixture(b2Fixture *fixture) {
        result_.append(convert_ptr(fixture));
        return true;
    }
};

boost::python::list query_aabb_2(b2World *world,
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
    boost::python::class_<b2World>("b2World", boost::python::init<b2Vec2, bool>())
        .add_property("Destructionboost::python::Listener", boost::python::object(), &b2World::SetDestructionListener)
        .add_property("ContactFilter", boost::python::object(), &b2World::SetContactFilter)
        .add_property("Contactboost::python::Listener", boost::python::object(), &b2World::SetContactListener)
        .def("DebugDraw", &b2World::SetDebugDraw)
        .add_property("Bodies", &get_bodies)
        .add_property("Joints", &get_joints)
        .add_property("Contactboost::python::List", make_function(&b2World::GetContactList, boost::python::return_internal_reference<>()))
        .add_property("WarmStarting", boost::python::object(), &b2World::SetWarmStarting)
        .add_property("ContinuousPhysics", boost::python::object(), &b2World::SetContinuousPhysics)
        .add_property("ProxyCount", &b2World::GetProxyCount)
        .add_property("BodyCount", &b2World::GetBodyCount)
        .add_property("JointCount", &b2World::GetJointCount)
        .add_property("ContactCount", &b2World::GetContactCount)
        .add_property("Gravity", &b2World::GetGravity, &b2World::SetGravity)
        .add_property("Locked", &b2World::IsLocked)
        .add_property("AutoClearForces", &b2World::GetAutoClearForces, &b2World::SetAutoClearForces)

        .def("CreateBody", &create_body_1, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("type"),
              boost::python::arg("position")=b2Vec2(0.0f, 0.0f),
              boost::python::arg("angle")=0.0f,
              boost::python::arg("linear_velocity")=b2Vec2(0.0f, 0.0f),
              boost::python::arg("angular_velocity")=0.0f,
              boost::python::arg("linear_damping")=0.0f,
              boost::python::arg("angular_damping")=0.0f,
              boost::python::arg("allow_sleep")=true,
              boost::python::arg("awake")=true,
              boost::python::arg("fixed_rotation")=false,
              boost::python::arg("bullet")=false,
              boost::python::arg("active")=true,
              boost::python::arg("inertia_scale")=1.0f))
        .def("CreateStaticBody", &create_body_2<b2_staticBody>, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("position")=b2Vec2(0.0f, 0.0f),
              boost::python::arg("angle")=0.0f,
              boost::python::arg("linear_velocity")=b2Vec2(0.0f, 0.0f),
              boost::python::arg("angular_velocity")=0.0f,
              boost::python::arg("linear_damping")=0.0f,
              boost::python::arg("angular_damping")=0.0f,
              boost::python::arg("allow_sleep")=true,
              boost::python::arg("awake")=true,
              boost::python::arg("fixed_rotation")=false,
              boost::python::arg("bullet")=false,
              boost::python::arg("active")=true,
              boost::python::arg("inertia_scale")=1.0f))
        .def("CreateKinematicBody", &create_body_2<b2_kinematicBody>, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("position")=b2Vec2(0.0f, 0.0f),
              boost::python::arg("angle")=0.0f,
              boost::python::arg("linear_velocity")=b2Vec2(0.0f, 0.0f),
              boost::python::arg("angular_velocity")=0.0f,
              boost::python::arg("linear_damping")=0.0f,
              boost::python::arg("angular_damping")=0.0f,
              boost::python::arg("allow_sleep")=true,
              boost::python::arg("awake")=true,
              boost::python::arg("fixed_rotation")=false,
              boost::python::arg("bullet")=false,
              boost::python::arg("active")=true,
              boost::python::arg("inertia_scale")=1.0f))
        .def("CreateDynamicBody", &create_body_2<b2_dynamicBody>, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("position")=b2Vec2(0.0f, 0.0f),
              boost::python::arg("angle")=0.0f,
              boost::python::arg("linear_velocity")=b2Vec2(0.0f, 0.0f),
              boost::python::arg("angular_velocity")=0.0f,
              boost::python::arg("linear_damping")=0.0f,
              boost::python::arg("angular_damping")=0.0f,
              boost::python::arg("allow_sleep")=true,
              boost::python::arg("awake")=true,
              boost::python::arg("fixed_rotation")=false,
              boost::python::arg("bullet")=false,
              boost::python::arg("active")=true,
              boost::python::arg("inertia_scale")=1.0f))
        .def("DestroyBody", &b2World::DestroyBody)
        .def("ReateRevoluteJoint", &create_revolute_joint, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("body_a"),
              boost::python::arg("body_b"),
              boost::python::arg("anchor"),
              boost::python::arg("limit_enabled")=false,
              boost::python::arg("lower_angle")=0.0f,
              boost::python::arg("upper_angle")=0.0f,
              boost::python::arg("motor_enabled")=false,
              boost::python::arg("motor_speed")=0.0f,
              boost::python::arg("max_motor_torque")=0.0f,
              boost::python::arg("collide_connected")=false))
        .def("CreatePrismaticJoint", &create_prismatic_joint, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("body_a"),
              boost::python::arg("body_b"),
              boost::python::arg("anchor"),
              boost::python::arg("axis"),
              boost::python::arg("limit_enabled")=false,
              boost::python::arg("lower_translation")=0.0f,
              boost::python::arg("upper_translation")=0.0f,
              boost::python::arg("motor_enabled")=false,
              boost::python::arg("max_motor_force")=0.0f,
              boost::python::arg("motor_speed")=0.0f,
              boost::python::arg("collide_connected")=false))
        .def("CreateDistanceJoint", &create_distance_joint, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("body_a"),
              boost::python::arg("body_b"),
              boost::python::arg("anchor_a"),
              boost::python::arg("anchor_b"),
              boost::python::arg("frequency")=0.0f,
              boost::python::arg("damping_ratio")=0.0f,
              boost::python::arg("collide_connected")=false))
        .def("CreatePulleyJoint", &create_pulley_joint, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("body_a"),
              boost::python::arg("body_b"),
              boost::python::arg("ground_anchor_a"),
              boost::python::arg("ground_anchor_b"),
              boost::python::arg("anchor_a"),
              boost::python::arg("anchor_b"),
              boost::python::arg("ratio")=0.0f,
              boost::python::arg("collide_connected")=false))
        .def("CreateMouseJoint", &create_mouse_joint, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("body"),
              boost::python::arg("tboost::python::arget"),
              boost::python::arg("max_force")=0.0f,
              boost::python::arg("frequency")=5.0f,
              boost::python::arg("damping_ratio")=0.7f,
              boost::python::arg("collide_connected")=false))
        .def("CreateGearJoint", &create_gear_joint, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("joint_a"),
              boost::python::arg("joint_b"),
              boost::python::arg("ratio"),
              boost::python::arg("collide_connected")=false))
        .def("CreateLineJoint", &create_line_joint, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("body_a"),
              boost::python::arg("body_b"),
              boost::python::arg("anchor"),
              boost::python::arg("axis"),
              boost::python::arg("limit_enabled")=false,
              boost::python::arg("lower_translation")=0.0f,
              boost::python::arg("upper_translation")=0.0f,
              boost::python::arg("motor_enabled")=false,
              boost::python::arg("max_motor_force")=0.0f,
              boost::python::arg("motor_speed")=0.0f,
              boost::python::arg("collide_connected")=false))
        .def("CreateWeldJoint", &create_weld_joint, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("body_a"),
              boost::python::arg("body_b"),
              boost::python::arg("anchor"),
              boost::python::arg("collide_connected")=false))
        .def("CreateFrictionJoint", &create_friction_joint, boost::python::return_internal_reference<>(),
             (boost::python::arg("self"),
              boost::python::arg("body_a"),
              boost::python::arg("body_b"),
              boost::python::arg("anchor"),
              boost::python::arg("max_force")=0.0f,
              boost::python::arg("max_force")=0.0f,
              boost::python::arg("collide_connected")=false))
        .def("DestroyJoint", &b2World::DestroyJoint)
        .def("Step", &b2World::Step)
        .def("ClearForces", &b2World::ClearForces)
        .def("DrawDebugData", &b2World::DrawDebugData)
        .def("QueryAABB", &query_aabb_1)
        .def("QueryAABB", &query_aabb_2)
        .def("RayCast", &b2World::RayCast)
    ;
}
