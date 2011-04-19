#include <boost/python.hpp>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2Shape.h>

#include "Scripts/Physic/SharedArrayRange.hpp"
#include "Scripts/Physic/b2Shape.hpp"

boost::shared_ptr<b2CircleShape> construct_circle_shape(b2Vec2 position, float32 radius)
{
    boost::shared_ptr<b2CircleShape> circle_shape(new b2CircleShape);
    circle_shape->m_p = position;
    circle_shape->m_radius = radius;
    return circle_shape;
}

boost::python::list get_polygon_shape_vertices(b2PolygonShape *polygon_shape)
{
    boost::python::list vertices;
    int32 n = polygon_shape->GetVertexCount();
    for (int32 i = 0; i != n; ++i) {
        vertices.append(polygon_shape->GetVertex(i));
    }
    return vertices;
}

void set_polygon_shape_vertices(b2PolygonShape *polygon_shape, boost::python::list vertices)
{
    b2Vec2 arr[b2_maxPolygonVertices];
    long n = len(vertices);
    for (long i = 0; i != n; ++i) {
        arr[i] = boost::python::extract<b2Vec2 &>(vertices[i]);
    }
    polygon_shape->Set(arr, n);
}

boost::shared_ptr<b2PolygonShape> construct_polygon_shape(boost::python::list vertices)
{
    boost::shared_ptr<b2PolygonShape> polygon_shape(new b2PolygonShape);
    set_polygon_shape_vertices(polygon_shape.get(), vertices);
    return polygon_shape;
}

void wrap_mass_data()
{
    boost::python::class_<b2MassData>("b2MassData")
        .def_readwrite("Mass", &b2MassData::mass)
        .def_readwrite("Center", &b2MassData::center)
        .def_readwrite("Inertia", &b2MassData::I)
    ;
}

void wrap_shape_type()
{
    boost::python::enum_<b2Shape::Type>("b2ShapeType")
        .value("UNKNOWN_SHAPE", b2Shape::e_unknown)
        .value("CIRCLE_SHAPE", b2Shape::e_circle)
        .value("POLYGON_SHAPE", b2Shape::e_polygon)
        .export_values()
    ;
}

void wrap_shape()
{
    boost::python::class_<b2Shape, boost::noncopyable>("b2Shape", boost::python::no_init)
        .add_property("Type", &b2Shape::GetType)
        .def_readwrite("Radius", &b2Shape::m_radius)

        .def("Clone", boost::python::pure_virtual(&b2Shape::Clone), boost::python::return_value_policy<boost::python::manage_new_object>())
        .def("GetType", &b2Shape::GetType)
        .def("TestPoint", boost::python::pure_virtual(&b2Shape::TestPoint))
        .def("RayCast", boost::python::pure_virtual(&b2Shape::RayCast))
        .def("ComputeAABB", boost::python::pure_virtual(&b2Shape::ComputeAABB))
        .def("ComputeMass", boost::python::pure_virtual(&b2Shape::ComputeMass))
        .def("GetRadius", boost::python::make_getter(&b2Shape::m_radius))
        .def("SetRadius", boost::python::make_setter(&b2Shape::m_radius))
    ;
}

void wrap_circle_shape()
{
    boost::python::class_<b2CircleShape, boost::python::bases<b2Shape> >("b2CircleShape")
        .add_property("VertexCount", &b2CircleShape::GetVertexCount)
        .add_property("Position", boost::python::make_getter(&b2CircleShape::m_p, boost::python::return_value_policy<boost::python::return_by_value>()), boost::python::make_setter(&b2CircleShape::m_p, boost::python::return_value_policy<boost::python::return_by_value>()))
        .def("__init__", boost::python::make_constructor(construct_circle_shape))
        .def("GetSupport", &b2CircleShape::GetSupport)
        .def("GetSupportVertex", &b2CircleShape::GetSupportVertex, boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("GetVertexCount", &b2CircleShape::GetVertexCount)
        .def("GetVertex", &b2CircleShape::GetVertex, boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("GetPosition", boost::python::make_getter(&b2CircleShape::m_p, boost::python::return_value_policy<boost::python::return_by_value>()))
        .def("SetPosition", boost::python::make_setter(&b2CircleShape::m_p, boost::python::return_value_policy<boost::python::return_by_value>()))
    ;
}

void wrap_polygon_shape()
{
    boost::python::class_<b2PolygonShape, boost::python::bases<b2Shape> >("b2PolygonShape")
        .add_property("VertexCount", &b2PolygonShape::GetVertexCount)
        .add_property("Vertices", &get_polygon_shape_vertices, &set_polygon_shape_vertices)

        .def("__init__", make_constructor(construct_polygon_shape))
        .def("GetVertexCount", &b2PolygonShape::GetVertexCount)
        .def("GetVertex", &b2PolygonShape::GetVertex, boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("GetVertices", &get_polygon_shape_vertices)
        .def("SetVertices", &set_polygon_shape_vertices)
    ;
}
