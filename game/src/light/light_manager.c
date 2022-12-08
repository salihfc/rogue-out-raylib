#include "raylib.h"
#include "light.c"

static const int STARTING_LIGHT_CAPACITY = 16;
typedef struct LightManager
{
	Light *lights;
	int light_count;
	int light_capacity;
} LightManager;

static void InitLightManager(LightManager *light_manager)
{
	light_manager->lights = (Light *)MemAlloc(sizeof(Light) * STARTING_LIGHT_CAPACITY);
	light_manager->light_count = 0;
}

static Light *GetNewLight(LightManager *light_manager)
{
	assert(light_manager->light_capacity > light_manager->light_count); // cap control > todo: add expansion
	return &light_manager->lights[light_manager->light_count++];
}

static void SetShaderLights(Shader shader)
{
}