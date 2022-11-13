#include "raylib.h"


static
float ease_smoothstep(float t)
{
	return (3.0 * t * t) - (2.0 * t * t * t);
}


static
float ease2_smoothstep(float t)
{
	return ease_smoothstep(ease_smoothstep(t));
}