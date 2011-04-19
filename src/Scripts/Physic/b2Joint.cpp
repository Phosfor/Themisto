#include "Scripts/Physic/Operator.hpp"

#include <boost/python.hpp>
#include <Box2D/Dynamics/b2Body.h>
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

void wrap_joint_type()
{
    enum_<b2JointType>("b2JointType")
        .value("UNKNOWN_JOINT", e_unknownJoint)
        .value("REVOLUTE_JOINT", e_revoluteJoint)
        .value("PRISMATIC_JOINT", e_prismaticJoint)
        .value("DISTANCE_JOINT", e_distanceJoint)
        .value("PULLEY_JOINT", e_pulleyJoint)
        .value("MOUSE_JOINT", e_mouseJoint)
        .value("GEAR_JOINT", e_gearJoint)
        .value("LINE_JOINT", e_lineJoint)
        .value("WELD_JOINT", e_weldJoint)
        .value("FRICTION_JOINT", e_frictionJoint)
        .export_values()
    ;
}

void wrap_joint()
{
    bp::class_<b2Joint, boost::noncopyable>("b2Joint", bp::no_init)
        .def("__eq__", &eq_ptr<b2Joint>)
        .def("__ne__", &ne_ptr<b2Joint>)
        .def("__hash__", &hash_ptr<b2Joint>)

        .add_property("Type", &b2Joint::GetType)
        .add_property("BodyA", make_function(&b2Joint::GetBodyA, return_internal_reference<>()))
        .add_property("BodyB", make_function(&b2Joint::GetBodyB, return_internal_reference<>()))
        .add_property("UserData", &b2Joint::GetUserData, &b2Joint::SetUserData)
        .add_property("Active", &b2Joint::IsActive)
    ;
}

void wrap_revolute_joint()
{
    bp::class_<b2RevoluteJoint, bp::bases<b2Joint> >("b2RevoluteJoint", bp::no_init)
        .add_property("AnchorA", &b2RevoluteJoint::GetAnchorA)
        .add_property("AnchorB", &b2RevoluteJoint::GetAnchorB)
        .add_property("ReactionForce", &b2RevoluteJoint::GetReactionForce)
        .add_property("ReactionTorque", &b2RevoluteJoint::GetReactionTorque)
        .add_property("JointAngle", &b2RevoluteJoint::GetJointAngle)
        .add_property("JointSpeed", &b2RevoluteJoint::GetJointSpeed)
        .add_property("LimitEnabled", &b2RevoluteJoint::IsLimitEnabled, &b2RevoluteJoint::EnableLimit)
        .add_property("LowerLimit", &b2RevoluteJoint::GetLowerLimit)
        .add_property("UpperLimit", &b2RevoluteJoint::GetUpperLimit)
        .add_property("MotorEnabled", &b2RevoluteJoint::IsMotorEnabled, &b2RevoluteJoint::EnableMotor)
        .add_property("MotorSpeed", &b2RevoluteJoint::GetMotorSpeed, &b2RevoluteJoint::SetMotorSpeed)
        .add_property("MotorTorque", &b2RevoluteJoint::GetMotorTorque)
        .add_property("MaxMotorTorque", object(), &b2RevoluteJoint::SetMaxMotorTorque)

        .def("SetLimits", &b2RevoluteJoint::SetLimits)
    ;
}

void wrap_prismatic_joint()
{
    bp::class_<b2PrismaticJoint, bp::bases<b2Joint> >("b2PrismaticJoint", bp::no_init)
        .add_property("AnchorA", &b2PrismaticJoint::GetAnchorA)
        .add_property("AnchorB", &b2PrismaticJoint::GetAnchorB)
        .add_property("ReactionForce", &b2PrismaticJoint::GetReactionForce)
        .add_property("ReactionTorque", &b2PrismaticJoint::GetReactionTorque)
        .add_property("JointTranslation", &b2PrismaticJoint::GetJointTranslation)
        .add_property("JointSpeed", &b2PrismaticJoint::GetJointSpeed)
        .add_property("LimitEnabled", &b2PrismaticJoint::IsLimitEnabled, &b2PrismaticJoint::EnableLimit)
        .add_property("LowerLimit", &b2PrismaticJoint::GetLowerLimit)
        .add_property("UpperLimit", &b2PrismaticJoint::GetUpperLimit)
        .add_property("MotorEnabled", &b2PrismaticJoint::IsMotorEnabled, &b2PrismaticJoint::EnableMotor)
        .add_property("MotorSpeed", &b2PrismaticJoint::GetMotorSpeed, &b2PrismaticJoint::SetMotorSpeed)
        .add_property("MotorForce", &b2PrismaticJoint::GetMotorForce)
        .add_property("MaxMotorForce", object(), &b2PrismaticJoint::SetMaxMotorForce)

        .def("SetLimits", &b2PrismaticJoint::SetLimits)
    ;
}

void wrap_distance_joint()
{
    bp::class_<b2DistanceJoint, bp::bases<b2Joint> >("b2DistanceJoint", bp::no_init)
        .add_property("AnchorA", &b2DistanceJoint::GetAnchorA)
        .add_property("AnchorB", &b2DistanceJoint::GetAnchorB)
        .add_property("ReactionForce", &b2PrismaticJoint::GetReactionForce)
        .add_property("ReactionTorque", &b2PrismaticJoint::GetReactionTorque)
        .add_property("Length", &b2DistanceJoint::GetLength, &b2DistanceJoint::SetLength)
        .add_property("Frequency", &b2DistanceJoint::GetFrequency, &b2DistanceJoint::SetFrequency)
        .add_property("DampingRatio", &b2DistanceJoint::GetDampingRatio, &b2DistanceJoint::SetDampingRatio)
    ;
}

void wrap_pulley_joint()
{
    bp::class_<b2PulleyJoint, bp::bases<b2Joint> >("b2PulleyJoint", bp::no_init)
        .add_property("AnchorA", &b2PulleyJoint::GetAnchorA)
        .add_property("AnchorB", &b2PulleyJoint::GetAnchorB)
        .add_property("ReactionForce", &b2PrismaticJoint::GetReactionForce)
        .add_property("ReactionTorque", &b2PrismaticJoint::GetReactionTorque)
        .add_property("GroundAnchorA", &b2PulleyJoint::GetGroundAnchorA)
        .add_property("GroundAnchorB", &b2PulleyJoint::GetGroundAnchorB)
        .add_property("Length1", &b2PulleyJoint::GetLength1)
        .add_property("Length2", &b2PulleyJoint::GetLength2)
        .add_property("Ratio", &b2PulleyJoint::GetRatio)
    ;
}

void wrap_mouse_joint()
{
    bp::class_<b2MouseJoint, bp::bases<b2Joint> >("b2MouseJoint", bp::no_init)
        .add_property("AnchorA", &b2MouseJoint::GetAnchorA)
        .add_property("AnchorB", &b2MouseJoint::GetAnchorB)
        .add_property("ReactionForce", &b2MouseJoint::GetReactionForce)
        .add_property("ReactionTorque", &b2MouseJoint::GetReactionTorque)
        .add_property("Target", make_function(&b2MouseJoint::GetTarget, return_value_policy<copy_const_reference>()), &b2MouseJoint::SetTarget)
        .add_property("MaxForce", &b2MouseJoint::GetMaxForce, &b2MouseJoint::SetMaxForce)
        .add_property("Frequency", &b2MouseJoint::GetFrequency, &b2MouseJoint::SetFrequency)
        .add_property("DampingRatio", &b2MouseJoint::GetDampingRatio, &b2MouseJoint::SetDampingRatio)
    ;
}

void wrap_gear_joint()
{
    bp::class_<b2GearJoint, bp::bases<b2Joint> >("b2GearJoint", bp::no_init)
        .add_property("AnchorA", &b2GearJoint::GetAnchorA)
        .add_property("AnchorB", &b2GearJoint::GetAnchorB)
        .add_property("ReactionForce", &b2GearJoint::GetReactionForce)
        .add_property("ReactionTorque", &b2GearJoint::GetReactionTorque)
        .add_property("Ratio", &b2GearJoint::GetRatio, &b2GearJoint::SetRatio)
    ;
}

void wrap_line_joint()
{
    bp::class_<b2LineJoint, bp::bases<b2Joint> >("b2LineJoint", bp::no_init)
        .add_property("AnchorA", &b2LineJoint::GetAnchorA)
        .add_property("AnchorB", &b2LineJoint::GetAnchorB)
        .add_property("ReactionForce", &b2LineJoint::GetReactionForce)
        .add_property("ReactionTorque", &b2LineJoint::GetReactionTorque)
        .add_property("JointTranslation", &b2LineJoint::GetJointTranslation)
        .add_property("JointSpeed", &b2LineJoint::GetJointSpeed)
        .add_property("LimitEnabled", &b2LineJoint::IsLimitEnabled, &b2LineJoint::EnableLimit)
        .add_property("LowerLimit", &b2LineJoint::GetLowerLimit)
        .add_property("UpperLimit", &b2LineJoint::GetUpperLimit)
        .add_property("MotorEnabled", &b2LineJoint::IsMotorEnabled, &b2LineJoint::EnableMotor)
        .add_property("MotorSpeed", &b2LineJoint::GetMotorSpeed, &b2LineJoint::SetMotorSpeed)
        .add_property("MotorForce", &b2LineJoint::GetMotorForce)
        .add_property("MaxMotorForce", object(), &b2LineJoint::SetMaxMotorForce)

        .def("set_limits", &b2LineJoint::SetLimits)
    ;
}

void wrap_weld_joint()
{
    bp::class_<b2WeldJoint, bp::bases<b2Joint> >("b2WeldJoint", bp::no_init)
        .add_property("AnchorA", &b2WeldJoint::GetAnchorA)
        .add_property("AnchorB", &b2WeldJoint::GetAnchorB)
        .add_property("ReactionForce", &b2WeldJoint::GetReactionForce)
        .add_property("ReactionTorque", &b2WeldJoint::GetReactionTorque)
    ;
}

void wrap_friction_joint()
{
    bp::class_<b2FrictionJoint, bp::bases<b2Joint> >("b2FrictionJoint", bp::no_init)
        .add_property("AnchorA", &b2FrictionJoint::GetAnchorA)
        .add_property("AnchorB", &b2FrictionJoint::GetAnchorB)
        .add_property("ReactionForce", &b2FrictionJoint::GetReactionForce)
        .add_property("ReactionTorque", &b2FrictionJoint::GetReactionTorque)
        .add_property("MaxForce", &b2FrictionJoint::GetMaxForce, &b2FrictionJoint::SetMaxForce)
        .add_property("MaxTorque", &b2FrictionJoint::GetMaxTorque, &b2FrictionJoint::SetMaxTorque)
    ;
}
