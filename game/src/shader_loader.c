#include "raylib.h"
#include "rlgl.h"

#define GLSL_VERSION 330

#ifndef __SHADER_LOADER
#define __SHADER_LOADER

static const char *PATH_TO_TEXTURE_64_WHITE = "game/resources/textures/64x64_white.png";

typedef struct ShaderLoader
{
	const char *fragment_filename;
	const char *vertex_filename;

	bool use_vertex;
	bool use_frag;

	Shader shader;

} ShaderLoader;

static void SetShaderParam(Shader shader, const char *param_name, void *param_pointer, int param_type)
{
	int loc = GetShaderLocation(shader, param_name);
	assert(loc != -1); // -1 means not found
	SetShaderValue(shader, loc, param_pointer, param_type);
}

static void SetShaderVec4(Shader shader, const char *param_name, Vector4 vec4)
{
	SetShaderParam(shader, param_name, (float[4]){vec4.x, vec4.y, vec4.z, vec4.w}, SHADER_UNIFORM_VEC4);
}

static void SetShaderVec2(Shader shader, const char *param_name, Vector2 vec2)
{
	SetShaderParam(shader, param_name, (float[2]){vec2.x, vec2.y}, SHADER_UNIFORM_VEC2);
}

static void SetShaderFloat(Shader shader, const char *param_name, float param_value)
{
	SetShaderParam(shader, param_name, &param_value, SHADER_UNIFORM_FLOAT);
}

static void SetShaderInt(Shader shader, const char *param_name, int param_value)
{
	SetShaderParam(shader, param_name, &param_value, SHADER_UNIFORM_INT);
}

static void SetShaderVec2Array(Shader shader, const char *param_name, float *floatArray, int size)
{
	int loc = GetShaderLocation(shader, param_name);
	SetShaderValueV(shader, loc, floatArray, SHADER_UNIFORM_VEC2, size);
}

static void SetShaderFloatArray(Shader shader, const char *param_name, float *floatArray, int size)
{
	int loc = GetShaderLocation(shader, param_name);
	SetShaderValueV(shader, loc, floatArray, SHADER_UNIFORM_FLOAT, size);
}

static void InitShaderLoader(ShaderLoader *shader_loader)
{
	if (!shader_loader->use_vertex && !shader_loader->use_frag)
		return;

	shader_loader->shader = LoadShader(
			(shader_loader->use_vertex) ? shader_loader->vertex_filename : 0,
			(shader_loader->use_frag) ? shader_loader->fragment_filename : 0);
}

static void InitShaderMode(ShaderLoader *shader_loader)
{
	// assert(shader_loader->use_vertex || shader_loader->use_frag);
	if (shader_loader->use_vertex || shader_loader->use_frag)
		BeginShaderMode(shader_loader->shader);
}

static void FinishShaderMode(ShaderLoader *shader_loader)
{
	// assert(shader_loader->use_vertex || shader_loader->use_frag);
	if (shader_loader->use_vertex || shader_loader->use_frag)
		EndShaderMode();
}

#endif