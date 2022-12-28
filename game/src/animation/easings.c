#include "raylib.h"

#ifndef __EASINGS
#define __EASINGS

typedef enum
{
	EASE_LINEAR,
	EASE_EXP,

	EASE_SMOOTH,
	EASE_SMOOTH2,

	EASE_CIRCLE,
	EASE_CIRCLE3,
	EASE_CIRCLE4,

	EASE_COUNT,
} EASING;

static float ease_smoothstep(float t)
{
	return (3.0 * t * t) - (2.0 * t * t * t);
}

static float ease2_smoothstep(float t)
{
	return ease_smoothstep(ease_smoothstep(t));
}

static float ease_circle(float t, float degree)
{
	return pow(1 - pow(t, degree), 1.0 / degree);
}

static float ease_exp(float t)
{
	float base = 100.0;
	return (pow(base, t) - 1) / (base - 1);
}

static float apply_easing(EASING easing_type, float t)
{
	switch (easing_type)
	{
	case EASE_LINEAR:
		return ease_circle(t, 1);
		break;

	case EASE_EXP:
		return ease_exp(t);
		break;

	case EASE_SMOOTH:
		return ease_smoothstep(t);
		break;

	case EASE_SMOOTH2:
		return ease2_smoothstep(t);
		break;

	case EASE_CIRCLE:
		return ease_circle(t, 2);
		break;

	case EASE_CIRCLE3:
		return ease_circle(t, 3);
		break;

	case EASE_CIRCLE4:
		return ease_circle(t, 4);
		break;

	default:
		break;
	}

	return t; // no easing
}

#endif