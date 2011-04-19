#include <boost/python.hpp>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2EdgeShape.h>
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

bp::list get_polygon_shape_vertices(b2PolygonShape *polygon_shape)
{
    bp::list vertices;
    int32 n = polygon_shape->GetVertexCount();
    for (int32 i = 0; i != n; ++i) {
        vertices.append(polygon_shape->GetVertex(i));
    }
    return vertices;
}

void set_polygon_shape_vertices(b2PolygonShape *polygon_shape, bp::list vertices)
{
    b2Vec2 arr[b2_maxPolygonVertices];
    long n = len(vertices);
    for (long i = 0; i != n; ++i) {
        arr[i] = extract<b2Vec2 &>(vertices[i]);
    }
    polygon_shape->Set(arr, n);
}

boost::shared_ptr<b2PolygonShape> construct_polygon_shape(bp::list vertices)
{
    boost::shared_ptr<b2PolygonShape> polygon_shape(new b2PolygonShape);
    set_polygon_shape_vertices(polygon_shape.get(), vertices);
    return polygon_shape;
}

void wrap_mass_data()
{
    class_<b2MassData>("b2MassData")
        .def_readwrite("Mass", &b2MassData::mass)
        .def_readwrite("Center", &b2MassData::center)
        .def_readwrite("Inertia", &b2MassData::I)
    ;
}

void wrap_shape_type()
{
    enum_<b2Shape::Type>("b2ShapeType")
        .value("UNKNOWN_SHAPE", b2Shape::e_unknown)
        .value("CIRCLE_SHAPE", b2Shape::e_circle)
        .value("EDGE_SHAPE", b2Shape::e_edge)
        .value("POLYGON_SHAPE", b2Shape::e_polygon)
        .value("LOOP_SHAPE", b2Shape::e_loop)
        .export_values()
    ;
}

void wrap_shape()
{
    class_<b2Shape, boost::noncopyable>("b2Shape", no_init)
        .add_property("Type", &b2Shape::GetType)
        .def_readwrite("Radius", &b2Shape::m_radius)

        .def("Clone", pure_virtual(&b2Shape::Clone), return_value_policy<manage_new_object>())
        .def("GetType", &b2Shape::GetType)
        .def("GetChildCount", pure_virtual(&b2Shape::GetChildCount))
        .def("TestPoint", pure_virtual(&b2Shape::TestPoint))
        .def("RayCast", pure_virtual(&b2Shape::RayCast))
        .def("ComputeAABB", pure_virtual(&b2Shape::ComputeAABB))
        .def("ComputeMass", pure_virtual(&b2Shape::ComputeMass))
        .def("GetRadius", make_getter(&b2Shape::m_radius))
        .def("SetRadius", make_setter(&b2Shape::m_radius))
    ;
}

void wrap_circle_shape()
{
    class_<b2CircleShape, bases<b2Shape> >("b2CircleShape")
        .add_property("ChildCount", &b2CircleShape::GetChildCount)
        .add_property("VertexCount", &b2CircleShape::GetVertexCount)
        .add_property("Position", make_getter(&b2CircleShape::m_p, return_value_policy<return_by_value>()), make_setter(&b2CircleShape::m_p, return_value_policy<return_by_value>()))
        .def("__init__", make_constructor(construct_circle_shape))
        .def("GetChildCount", &b2CircleShape::GetChildCount)
        .def("GetSupport", &b2CircleShape::GetSupport)
        .def("GetSupportVertex", &b2CircleShape::GetSupportVertex, return_value_policy<copy_const_reference>())
        .def("GetVertexCount", &b2CircleShape::GetVertexCount)
        .def("GetVertex", &b2CircleShape::GetVertex, return_value_policy<copy_const_reference>())
        .def("GetPosition", make_getter(&b2CircleShape::m_p, return_value_policy<return_by_value>()))
        .def("SetPosition", make_setter(&b2CircleShape::m_p, return_value_policy<return_by_value>()))
    ;
}

void wrap_edge_shape()
{
    class_<b2EdgeShape, bases<b2Shape> >("b2EdgeShape")
        .add_property("ChildCount", &b2EdgeShape::GetChildCount)
        .add_property("Vertex1", make_getter(&b2EdgeShape::m_vertex1, return_value_policy<return_by_value>()), make_setter(&b2EdgeShape::m_vertex1, return_value_policy<return_by_value>()))
        .add_property("Vertex2", make_getter(&b2EdgeShape::m_vertex2, return_value_policy<return_by_value>()), make_setter(&b2EdgeShape::m_vertex2, return_value_policy<return_by_value>()))

        .def("GetChildCount", &b2EdgeShape::GetChildCount)
        .def("GetVertex1", make_getter(&b2EdgeShape::m_vertex1, return_value_policy<return_by_value>()))
        .def("SetVertex1", make_setter(&b2EdgeShape::m_vertex1, return_value_policy<return_by_value>()))
        .def("GetVertex2", make_getter(&b2EdgeShape::m_vertex2, return_value_policy<return_by_value>()))
        .def("SetVertex2", make_setter(&b2EdgeShape::m_vertex2, return_value_policy<return_by_value>()))
    ;
}

void wrap_polygon_shape()
{
    class_<b2PolygonShape, bases<b2Shape> >("b2PolygonShape")
        .add_property("ChildCount", &b2PolygonShape::GetChildCount)
        .add_property("VertexCount", &b2PolygonShape::GetVertexCount)
        .add_property("Vertices", &get_polygon_shape_vertices, &set_polygon_shape_vertices)

        .def("__init__", make_constructor(construct_polygon_shape))
        .def("GetChildCount", &b2PolygonShape::GetChildCount)
        .def("GetVertexCount", &b2PolygonShape::GetVertexCount)
        .def("GetVertex", &b2PolygonShape::GetVertex, return_value_policy<copy_const_reference>())
        .def("GetVertices", &get_polygon_shape_vertices)
        .def("SetVertices", &set_polygon_shape_vertices)
    ;
}
