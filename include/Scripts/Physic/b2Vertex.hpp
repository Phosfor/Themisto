#pragma once

#include <Box2D/Box2D.h>
#include <boost/python.hpp>
#include "Scripts/Physic/SharedArrayRange.hpp"

typedef shared_array_range<b2Vec2> VertexArray;
boost::shared_ptr<VertexArray> construct_vertex_array(boost::python::list vertices);
b2Vec2 *vertex_array_getitem(VertexArray *b, std::size_t index);
void vertex_array_setitem(VertexArray *b, std::size_t index, b2Vec2 *v);
void wrap_vertex_array();
