#include "math.h"
#include "utils.c"
#include "vector2.c"

#ifndef _GEO_UTILS
#define _GEO_UTILS

static float Dot(Vector2 a, Vector2 b)
{
	return sqrt(a.x * b.x + a.y * b.y);
}

// p: point, segment: [a->b]
static float DistPointToSegment(Vector2 p, Vector2 a, Vector2 b)
{
	Vector2 ap = VectorDif(p, a);
	Vector2 ab = VectorDif(b, a);

	float h = Dot(ap, ab) / Dot(ab, ab);
	h = Clamp(h, 0, 1);

	Vector2 q = VectorSum(a, VectorScaled(ab, h));
	Vector2 pq = VectorDif(p, q);
	return Dot(pq, pq);
}

/*
	Center stays the same
		x, y, width, height updated accordingly
*/
static Rectangle ContractRectangle(Rectangle rect, float width_contract, float height_contract)
{
	rect.x += width_contract / 2.0;
	rect.y += height_contract / 2.0;

	rect.width -= width_contract;
	rect.height -= height_contract;

	return rect;
}

#endif