#pragma once

boost::shared_ptr<b2Vec2> construct();
int32 len(b2Vec2 *v);
float32 getitem(b2Vec2 *v, int32 index);
void setitem(b2Vec2 *v, int32 index, float32 value);
std::string repr(const b2Vec2 &v);
void wrap_vec_2();
