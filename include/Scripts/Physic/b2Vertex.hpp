#pragma once

#include "Scripts/Physic/SharedArrayRange.hpp"

typedef shared_array_range<b2Vec2> VertexArray;
boost::shared_ptr<VertexArray> construct_vertex_array(bp::list vertices);
b2Vec2 *vertex_array_getitem(VertexArray *b, std::size_t index);
void vertex_array_setitem(VertexArray *b, std::size_t index, b2Vec2 *v);
void wrap_vertex_array();
