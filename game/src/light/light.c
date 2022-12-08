#include "raylib.h"
#include "game/src/utils/geo_utils.c"

typedef struct Light
{
	Vector2 position;
	float intensity;
} Light;