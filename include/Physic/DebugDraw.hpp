#ifndef _PHYSIC_DEBUG_DRAW_HPP_
#define _PHYSIC_DEBUG_DRAW_HPP_

#include <Box2D/Box2D.h>
#include "Core/ApplicationManager.hpp"

struct b2AABB;
class DebugDraw : public b2DebugDraw
{
    private:
        CL_GraphicContext mGC;

        void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
        void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

        void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
        void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
        void DrawTransform(const b2Transform& xf);
        void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
        void DrawAABB(b2AABB* aabb, const b2Color& color);

    public:
        DebugDraw();
};

#endif /* _PHYSIC_DEBUG_DRAW_HPP_ */
