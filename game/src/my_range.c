#include "raylib.h"
#include "utils/utils.c"
#include "assert.h"
#include <math.h>

#ifndef __MY_RANGE
#define __MY_RANGE

typedef struct MyRange
{
	float min;
	float max;
} MyRange;

typedef enum Distribution
{
	UNIFORM_DISTRIBUTION = 0,
	NORMAL_DISTRIBUTION,

	DISTRIBUTION_COUNT
} Distribution;

static float GetRandomInRange(MyRange range, Distribution dist)
{
	switch (dist)
	{
	case UNIFORM_DISTRIBUTION:
		return GetRandomFloat(range.min, range.max);
		break;

		// case NORMAL_DISTRIBUTION:
		// 	float interval1 = range.max - range.min;
		// 	float sq_root = sqrt(interval1);
		// 	float r1 = GetRandomFloat(0.0, sq_root);
		// 	float r2 = GetRandomFloat(0.0, sq_root);
		// 	return range.min + r1 * r2;
		// 	break;

	default:
		break;
	}

	assert(0);
	return 0;
}

#endif