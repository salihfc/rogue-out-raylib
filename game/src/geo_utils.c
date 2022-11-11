#include "raylib.h"
#include "math.h"
#include "utils.c"


#ifndef _GEO_UTILS
#define _GEO_UTILS

// -------------
// VECTOR2 UTILS
// -------------
static inline
Vector2 VectorSum(Vector2 a, Vector2 b)
{
	return (Vector2) {
		a.x + b.x,
		a.y + b.y
	};
}

static inline
Vector2 VectorDif(Vector2 a, Vector2 b)
{
	return (Vector2) {
		a.x - b.x,
		a.y - b.y
	};
}

static inline
Vector2 VectorScaled(Vector2 v, float scale)
{
	return (Vector2) {
		v.x * scale,
		v.y * scale
	};
}
// -------------
// ~VECTOR2
// -------------


static
float Dot(Vector2 a, Vector2 b)
{
	return sqrt(a.x * b.x + a.y * b.y);
}


// p: point, segment: [a->b]
static
float DistPointToSegment(Vector2 p, Vector2 a, Vector2 b)
{
	Vector2 ap = VectorDif(p, a);
	Vector2 ab = VectorDif(b, a);

	float h = Dot(ap, ab) / Dot(ab, ab);
	h = Clamp(h, 0, 1);

	Vector2 q = VectorSum(a, VectorScaled(ab, h));
	Vector2 pq = VectorDif(p, q);
	return Dot(pq, pq);
}


#endif