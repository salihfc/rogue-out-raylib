#include "raylib.h"
#include "rlgl.h"

#define GLSL_VERSION 330

#ifndef __SHADER_LOADER
#define __SHADER_LOADER

typedef struct ShaderLoader
{
	const char *fragment_filename;
	const char *vertex_filename;

	bool use_vertex;
	bool use_frag;

	Shader shader;

} ShaderLoader;

static void SetShaderParam(Shader *shader, const char *param_name, void *param_pointer, int param_type)
{
	int loc = GetShaderLocation(*shader, param_name);
	assert(loc != -1); // -1 means not found
	SetShaderValue(*shader, loc, param_pointer, param_type);
}

static void InitShaderLoader(ShaderLoader *shader_loader)
{
	if (!shader_loader->use_vertex && !shader_loader->use_frag)
		return;

	shader_loader->shader = LoadShader(
			(shader_loader->use_vertex) ? shader_loader->vertex_filename : 0,
			(shader_loader->use_frag) ? shader_loader->fragment_filename : 0);

	// float color_hint[4] = {1.0f, 1.0f, 0.0f, 1.0f};
	// SetShaderParam(&shader_loader->base_shader, "color_hint", color_hint, SHADER_UNIFORM_VEC4);
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