#include "raylib.h"
#include "light.c"
#include "../shader_loader.c"

static const int STARTING_LIGHT_CAPACITY = 16;
static const char *LIGHT_POSITION_STR = "light_pos";
static const char *LIGHT_INTENSITY_STR = "light_intensity";
static const char *LIGHT_COUNT_STR = "light_count";

typedef struct LightManager
{
	UniLight *lights;
	int light_count;
	int light_capacity;

	float *light_position_array;
	bool position_dirty;

	float *light_intensity_array;
	bool intensity_dirty;

} LightManager;

static void InitLightManager(LightManager *light_manager)
{
	light_manager->light_capacity = STARTING_LIGHT_CAPACITY;
	light_manager->lights = (UniLight *)MemAlloc(sizeof(UniLight) * light_manager->light_capacity);
	light_manager->light_count = 0;
	light_manager->position_dirty = false;
	light_manager->intensity_dirty = false;
}

static UniLight *GetNewLight(LightManager *light_manager)
{
	assert(light_manager->light_capacity > light_manager->light_count); // cap control > todo: add expansion
	light_manager->position_dirty = true;
	light_manager->intensity_dirty = true;
	return &light_manager->lights[light_manager->light_count++];
}

static float *GetLightPositionArray(LightManager *light_manager)
{
	if (light_manager->position_dirty)
	{
		if (light_manager->light_position_array)
			light_manager->light_position_array = (float *)MemRealloc(light_manager->light_position_array, light_manager->light_count * 2 * sizeof(float));
		else
			light_manager->light_position_array = (float *)MemAlloc(light_manager->light_count * 2 * sizeof(float));

		light_manager->position_dirty = false;
	}

	for (int i = 0; i < light_manager->light_count; i++)
	{
		light_manager->light_position_array[2 * i] = light_manager->lights[i].position.x;
		light_manager->light_position_array[2 * i + 1] = light_manager->lights[i].position.y;
	}

	return light_manager->light_position_array;
}

static float *GetLightIntensityArray(LightManager *light_manager)
{
	if (light_manager->intensity_dirty)
	{
		if (light_manager->light_intensity_array)
			light_manager->light_intensity_array = (float *)MemRealloc(light_manager->light_intensity_array, light_manager->light_count * sizeof(float));
		else
			light_manager->light_intensity_array = (float *)MemAlloc(light_manager->light_count * sizeof(float));

		light_manager->intensity_dirty = false;
	}

	for (int i = 0; i < light_manager->light_count; i++)
		light_manager->light_intensity_array[i] = light_manager->lights[i].intensity;
	return light_manager->light_intensity_array;
}

static void SetShaderLights(LightManager *light_manager, Shader shader)
{
	float *light_pos_array = GetLightPositionArray(light_manager);
	SetShaderVec2Array(shader, LIGHT_POSITION_STR, light_pos_array, light_manager->light_count);

	float *light_intensity_array = GetLightIntensityArray(light_manager);
	SetShaderFloatArray(shader, LIGHT_INTENSITY_STR, light_intensity_array, light_manager->light_count);

	SetShaderInt(shader, LIGHT_COUNT_STR, light_manager->light_count);
}