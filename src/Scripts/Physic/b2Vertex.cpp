#include "Scripts/Physic/b2Vertex.hpp"

typedef shared_array_range<b2Vec2> VertexArray;
boost::shared_ptr<VertexArray> construct_vertex_array(bp::list vertices)
{
    long n = bp::len(vertices);
    boost::shared_array<b2Vec2> arr(new b2Vec2[n]);
    for (long i = 0; i != n; ++i) {
        arr[i] = bp::extract<b2Vec2 &>(vertices[i]);
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
    bp::class_<VertexArray>("VertexArray")
        .def("__init__", make_constructor(&construct_vertex_array))
        .def("__len__", &VertexArray::size)
        .def("__getitem__", &vertex_array_getitem, bp::return_internal_reference<>())
        .def("__setitem__", &vertex_array_setitem)
    ;
}
