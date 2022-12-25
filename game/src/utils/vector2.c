#include <raylib.h>

#ifndef _VECTOR2
#define _VECTOR2

typedef enum
{
	LEFT = 0,
	UP,
	RIGHT,
	DOWN,
	DIRECTION_COUNT
} Direction;

static inline const char *dir_to_string(Direction dir)
{
	switch (dir)
	{
	case UP:
		return "UP";
	case DOWN:
		return "DOWN";
	case LEFT:
		return "LEFT";
	case RIGHT:
		return "RIGHT";
	}
	return "[!INVALID DIRECTION]";
}

static inline Vector2 zero() { return (Vector2){0, 0}; }
static inline Vector2 left() { return (Vector2){-1, 0}; }
static inline Vector2 right() { return (Vector2){+1, 0}; }
static inline Vector2 up() { return (Vector2){0, -1}; }
static inline Vector2 down() { return (Vector2){0, 1}; }

static inline Vector2 negate(Vector2 v) { return (Vector2){-v.x, -v.y}; }
static inline float dot(Vector2 a, Vector2 b) { return a.x * b.x + a.y * b.y; }

static Vector2 get_dir_vector(Direction dir)
{
	switch (dir)
	{
	case LEFT:
		return left();
	case UP:
		return up();
	case RIGHT:
		return right();
	case DOWN:
		return down();
	}

	return zero();
}

// Get closest cardinal direction to surface with normal
static inline Direction get_direction(Vector2 normal)
{
	Direction max_dir;
	float maxx = -1e9;

	for (int dir = LEFT; dir < DIRECTION_COUNT; dir++)
	{
		Vector2 other = negate(get_dir_vector(dir));
		float dot_product = dot(normal, other);
		// printf("dot_prod: %f [%f %f, %f %f]\n", dot_product, normal.x, normal.y, other.x, other.y);

		if (maxx < dot_product)
		{
			maxx = dot_product;
			max_dir = dir;
		}
	}

	return max_dir;
}

static inline Vector2 VectorSum(Vector2 a, Vector2 b)
{
	return (Vector2){
			a.x + b.x,
			a.y + b.y};
}

static inline Vector2 VectorDif(Vector2 a, Vector2 b)
{
	return (Vector2){
			a.x - b.x,
			a.y - b.y};
}

static inline Vector2 VectorScaled(Vector2 v, float scale)
{
	return (Vector2){
			v.x * scale,
			v.y * scale};
}

static inline float VectorLen(Vector2 v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

static inline Vector2 VectorNormalized(Vector2 v)
{
	float len = VectorLen(v);
	return (Vector2){
			v.x / len,
			v.y / len};
}

static inline float VectorAngle(Vector2 v)
{
	return atan2f(v.y, v.x);
}

static inline Vector2 AngleToVector(float angle)
{
	return (Vector2){
			cosf(angle),
			sinf(angle)};
}

static inline Vector2 VectorRotated(Vector2 v, float angle)
{
	return Vector2Rotate(v, angle);
}

static inline Vector2 VectorDirectionTo(Vector2 from, Vector2 to)
{
	return VectorNormalized(VectorDif(to, from));
}

static inline Vector2 GetRandomVector2(float magnitude)
{
	double theta = (float)(GetRandomValue(0, 3600)) / 3600.0 * (2.0 * PI);
	return (Vector2){
			cos(theta) * magnitude,
			sin(theta) * magnitude};
}

#endif