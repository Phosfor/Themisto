#include <boost/python.hpp>
#include <Box2D/Dynamics/b2Fixture.h>

#include "Scripts/Physic/Operator.hpp"
#include "Scripts/Physic/b2Fixture.hpp"

uint16 get_fixture_category_bits(const b2Fixture *fixture)
{
    return fixture->GetFilterData().categoryBits;
}

void set_fixture_category_bits(b2Fixture *fixture, uint16 category_bits)
{
    b2Filter filter = fixture->GetFilterData();
    filter.categoryBits = category_bits;
    fixture->SetFilterData(filter);
}

uint16 get_fixture_mask_bits(const b2Fixture *fixture)
{
    return fixture->GetFilterData().maskBits;
}

void set_fixture_mask_bits(b2Fixture *fixture, uint16 mask_bits)
{
    b2Filter filter = fixture->GetFilterData();
    filter.maskBits = mask_bits;
    fixture->SetFilterData(filter);
}

uint16 get_fixture_group_index(const b2Fixture *fixture)
{
    return fixture->GetFilterData().groupIndex;
}

void set_fixture_group_index(b2Fixture *fixture, uint16 group_index)
{
    b2Filter filter = fixture->GetFilterData();
    filter.groupIndex = group_index;
    fixture->SetFilterData(filter);
}

void wrap_fixture()
{
    b2Shape *(b2Fixture::*get_shape)() = &b2Fixture::GetShape;
    b2Body *(b2Fixture::*get_body)() = &b2Fixture::GetBody;
    b2Fixture *(b2Fixture::*get_next)() = &b2Fixture::GetNext;

    bp::class_<b2Fixture, boost::noncopyable>("b2Fixture", bp::no_init)
        .def("__eq__", &eq_ptr<b2Fixture>)
        .def("__ne__", &ne_ptr<b2Fixture>)
        .def("__hash__", &hash_ptr<b2Fixture>)

        .add_property("type", &b2Fixture::GetType)
        .add_property("shape", make_function(get_shape, bp::return_internal_reference<>()))
        .add_property("sensor", &b2Fixture::IsSensor, &b2Fixture::SetSensor)
        .add_property("category_bits", &get_fixture_category_bits, &set_fixture_category_bits)
        .add_property("mask_bits", &get_fixture_mask_bits, &set_fixture_mask_bits)
        .add_property("group_index", &get_fixture_group_index, &set_fixture_group_index)
        .add_property("body", make_function(get_body, bp::return_internal_reference<>()))
        .add_property("next", make_function(get_next, bp::return_internal_reference<>()))
        .add_property("user_data", &b2Fixture::GetUserData, &b2Fixture::SetUserData)
        .def("test_point", &b2Fixture::TestPoint)
        .def("ray_cast", &b2Fixture::RayCast)
        .def("get_mass_data", &b2Fixture::GetMassData)
        .add_property("density", &b2Fixture::GetDensity, &b2Fixture::SetDensity)
        .add_property("friction", &b2Fixture::GetFriction, &b2Fixture::SetFriction)
        .add_property("restitution", &b2Fixture::GetRestitution, &b2Fixture::SetRestitution)
        .def("get_aabb", make_function(&b2Fixture::GetAABB, bp::return_value_policy<bp::copy_const_reference>()))
    ;
}
