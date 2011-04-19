#include "Scripts/Physic/b2Vertex.hpp"

typedef shared_array_range<b2Vec2> VertexArray;
boost::shared_ptr<VertexArray> construct_vertex_array(boost::python::list vertices)
{
    long n = boost::python::len(vertices);
    boost::shared_array<b2Vec2> arr(new b2Vec2[n]);
    for (long i = 0; i != n; ++i) {
        arr[i] = boost::python::extract<b2Vec2 &>(vertices[i]);
    }
    return boost::shared_ptr<VertexArray>(new VertexArray(arr, n));
}

b2Vec2 *vertex_array_getitem(VertexArray *b, std::size_t index)
{
    return &b->at(index);
}

void vertex_array_setitem(VertexArray *b, std::size_t index, b2Vec2 *v)
{
    b->at(index) = *v;
}

void wrap_vertex_array()
{
    boost::python::class_<VertexArray>("VertexArray")
        .def("__init__", boost::python::make_constructor(&construct_vertex_array))
        .def("__len__", &VertexArray::size)
        .def("__getitem__", &vertex_array_getitem, boost::python::return_internal_reference<>())
        .def("__setitem__", &vertex_array_setitem)
    ;
}
