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

static inline
float VectorLen(Vector2 v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}


static inline
Vector2 VectorNormalized(Vector2 v)
{
	float len = VectorLen(v);
	return (Vector2) {
		v.x / len,
		v.y / len
	};
}


static inline
float VectorAngle(Vector2 v)
{
	return atan2f(v.y, v.x);
}

static inline 
Vector2 AngleToVector(float angle)
{
	return (Vector2) {
		cosf(angle),
		sinf(angle)
	};
}


static inline
Vector2 VectorRotated(Vector2 v, float angle)
{
	float len = VectorLen(v);
	float v_angle = VectorAngle(v) + angle;
	return VectorScaled(
		AngleToVector(v_angle),
		len
	);
}


static inline
Vector2 VectorDirectionTo(Vector2 from, Vector2 to)
{
	return VectorNormalized(VectorDif(to, from));
}


static inline
Vector2 GetRandomVector2(float magnitude)
{
	double theta = (float)(GetRandomValue(0, 3600)) / 3600.0 * (2.0 * PI);
	return (Vector2) {
		cos(theta) * magnitude,
		sin(theta) * magnitude
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