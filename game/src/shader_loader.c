#include "raylib.h"
#include "rlgl.h"

#define GLSL_VERSION 330

typedef struct ShaderLoader
{
	const char *base_frag_shader_filename;
	const char *base_vertex_shader_filename;

	Shader base_shader;
	Texture2D base_texture;

} ShaderLoader;

static void SetShaderParam(Shader *shader, const char *param_name, void *param_pointer, int param_type)
{
	int loc = GetShaderLocation(*shader, param_name);
	assert(loc != -1); // -1 means not found
	SetShaderValue(*shader, loc, param_pointer, param_type);
}

static void InitShaderLoader(ShaderLoader *shader_loader)
{
	shader_loader->base_vertex_shader_filename = "game/shaders/base.vs";
	shader_loader->base_frag_shader_filename = "game/shaders/base.fs";
	shader_loader->base_shader = LoadShader(0, TextFormat(shader_loader->base_frag_shader_filename, GLSL_VERSION));

	shader_loader->base_texture = LoadTexture("game/resources/textures/raylib_64x64.png");

	float color_hint[4] = {1.0f, 1.0f, 0.0f, 1.0f};
	SetShaderParam(&shader_loader->base_shader, "color_hint", color_hint, SHADER_UNIFORM_VEC4);
}

static void DrawShader(ShaderLoader *shader_loader)
{
	BeginShaderMode(shader_loader->base_shader);
	{
		DrawTexture(shader_loader->base_texture, 500, 500, BLANK);
		// DrawRectangle(500, 500, 300, 300, BLANK);
	}
	EndShaderMode();
}
