#include <Box2D/Common/b2Math.h>
#include <Box2D/Common/b2Settings.h>

#include "Scripts/Physic/b2Body.hpp"
#include <Core/ScriptsManager.hpp>
#include "Physic/Operator.hpp"
#include "Physic/Convert.hpp"
#include "Physic/PhysicUtils.hpp"

void wrap_body_type()
{
    bp::enum_<b2BodyType>("b2BodyType")
        .value("STATIC_BODY", b2_staticBody)
        .value("KINEMATIC_BODY", b2_kinematicBody)
        .value("DYNAMIC_BODY", b2_dynamicBody)
        .export_values()
    ;
}

void wrap_body()
{
    b2ContactEdge *(b2Body::*get_contact_list)() = &b2Body::GetContactList;
    b2Body *(b2Body::*get_next)() = &b2Body::GetNext;
    b2World *(b2Body::*get_world)() = &b2Body::GetWorld;

    bp::class_<b2Body, boost::noncopyable>("b2Body", bp::no_init)
        .def("__eq__", &eq_ptr<b2Body>)
        .def("__ne__", &ne_ptr<b2Body>)
        .def("__hash__", &hash_ptr<b2Body>)

        .add_property("Transfrom", make_function(&b2Body::GetTransform, bp::return_value_policy<bp::copy_const_reference>()), &b2Body::SetTransform)
        .add_property("Position", make_function(&b2Body::GetPosition, bp::return_value_policy<bp::copy_const_reference>()))
        .add_property("Angle", &b2Body::GetAngle)
        .add_property("WorldCenter", make_function(&b2Body::GetWorldCenter, bp::return_value_policy<bp::copy_const_reference>()))
        .add_property("LocalCenter", make_function(&b2Body::GetLocalCenter, bp::return_value_policy<bp::copy_const_reference>()))
        .add_property("LinearVelocity", &b2Body::GetLinearVelocity, &b2Body::SetLinearVelocity)
        .add_property("AngularVelocity", &b2Body::GetAngularVelocity, &b2Body::SetAngularVelocity)
        .add_property("Mass", &b2Body::GetMass)
        .add_property("Inertia", &b2Body::GetInertia)
        .add_property("LinearDamping", &b2Body::GetLinearDamping, &b2Body::SetLinearDamping)
        .add_property("AngularDamping", &b2Body::GetAngularDamping, &b2Body::SetAngularDamping)
        .add_property("Type", &b2Body::GetType, &b2Body::SetType)
        .add_property("Bullet", &b2Body::IsBullet, &b2Body::SetBullet)
        .add_property("SleepingAllowed", &b2Body::IsSleepingAllowed, &b2Body::SetSleepingAllowed)
        .add_property("Awake", &b2Body::IsAwake, &b2Body::SetAwake)
        .add_property("Active", &b2Body::IsActive, &b2Body::SetActive)
        .add_property("FixedRotation", &b2Body::IsFixedRotation, &b2Body::SetFixedRotation)
        .add_property("Fixtures", &get_fixtures)
        .add_property("Joints", &get_joints)
        .add_property("ContactList", make_function(get_contact_list, bp::return_internal_reference<>()))
        .add_property("Next", make_function(get_next, bp::return_internal_reference<>()))
        //.add_property("UserData", make_function(&b2Body::GetUserData, bp::return_internal_reference<>()), &b2Body::SetUserData)
        .add_property("World", make_function(get_world, bp::return_internal_reference<>()))

        .def("CreateFixture", &create_fixture, bp::return_internal_reference<>(),
             (bp::arg("bp::self"),
              bp::arg("shape"),
              bp::arg("user_data")=bp::object(),
              bp::arg("friction")=0.2f,
              bp::arg("restitution")=0.0f,
              bp::arg("density")=0.0f,
              bp::arg("sensor")=false,
              bp::arg("category_bits")=0x0001,
              bp::arg("mask_bits")=0xffff,
              bp::arg("group_index")=0))
        .def("CreateCircleFixture", &create_circle_fixture, bp::return_internal_reference<>(),
             (bp::arg("bp::self"),
              bp::arg("position")=b2Vec2(0.0f, 0.0f),
              bp::arg("radius")=0.5f,
              bp::arg("user_data")=bp::object(),
              bp::arg("friction")=0.2f,
              bp::arg("restitution")=0.0f,
              bp::arg("density")=0.0f,
              bp::arg("sensor")=false,
              bp::arg("category_bits")=0x0001,
              bp::arg("mask_bits")=0xffff,
              bp::arg("group_index")=0))
        .def("CreatePolygonFixture", &create_polygon_fixture, bp::return_internal_reference<>(),
             (bp::arg("bp::self"),
              bp::arg("vertices")=bp::list(),
              bp::arg("user_data")=bp::object(),
              bp::arg("friction")=0.2f,
              bp::arg("restitution")=0.0f,
              bp::arg("density")=0.0f,
              bp::arg("sensor")=false,
              bp::arg("category_bits")=0x0001,
              bp::arg("mask_bits")=0xffff,
              bp::arg("group_index")=0))
        .def("CreateBoxFixture", &create_box_fixture, bp::return_internal_reference<>(),
             (bp::arg("bp::self"),
              bp::arg("half_width")=0.5f,
              bp::arg("half_height")=0.5f,
              bp::arg("center")=b2Vec2(0.0f, 0.0f),
              bp::arg("angle")=0.0f,
              bp::arg("user_data")=bp::object(),
              bp::arg("friction")=0.2f,
              bp::arg("restitution")=0.0f,
              bp::arg("density")=0.0f,
              bp::arg("sensor")=false,
              bp::arg("category_bits")=0x0001,
              bp::arg("mask_bits")=0xffff,
              bp::arg("group_index")=0))
        .def("DestroyFixture", &b2Body::DestroyFixture)
        .def("ApplyForce", &b2Body::ApplyForce)
        .def("ApplyTorque", &b2Body::ApplyTorque)
        .def("ApplyLinearImpulse", &b2Body::ApplyLinearImpulse)
        .def("ApplyAngularImpulse", &b2Body::ApplyAngularImpulse)
        .def("SetMassData", &set_mass_data)
        .def("ResetMassData", &b2Body::ResetMassData)
        .def("Getworldpoint", &b2Body::GetWorldPoint)
        .def("GetWorldVector", &b2Body::GetWorldVector)
        .def("GetLocalPoint", &b2Body::GetLocalPoint)
        .def("GetLocalVector", &b2Body::GetLocalVector)
        .def("GetLinearVelocityFromWorldPoint", &b2Body::GetLinearVelocityFromWorldPoint)
        .def("GetLinearVelocityFromLocalPoint", &b2Body::GetLinearVelocityFromLocalPoint)
    ;
}
