#include "raylib.h"
#include "../utils/geo_utils.c"

#ifndef __UNI_LIGHT
#define __UNI_LIGHT

typedef struct UniLight
{
	Vector2 position;
	float intensity;
} UniLight;

#endif