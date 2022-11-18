#include "raylib.h"
#include "utils.c"
#include "assert.h"

typedef
struct Range
{
	float min;
	float max;
} Range;

typedef
enum {
	UNIFORM = 0,
	NORMAL,

	DISTRIBUTION_COUNT
} Distribution;


static
float GetRandomInRange(Range range, Distribution dist)
{
	switch (dist)
	{
	case UNIFORM:
		return GetRandomFloat(range.min, range.max);
		break;

	case NORMAL:
		float interval = range.max - range.min;
		float sq_root = sqrt(interval);
		float r1 = GetRandomFloat(0.0, sq_root);
		float r2 = GetRandomFloat(0.0, sq_root);
		return range.min + r1 * r2;
		break;
	
	default:
		break;
	}

	assert(0);
	return 0;
}