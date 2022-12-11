#include "raylib.h"
#include "raymath.h"
#include "../shader_loader.c"

typedef struct SDFObject
{
	ShaderLoader shader_loader;
	Texture2D texture;
} SDFObject;

static void InitSDF(SDFObject *sdf)
{
	sdf->shader_loader = (ShaderLoader){
			.use_frag = true,
			.fragment_filename = "game/shaders/sdf.fs"};

	InitShaderLoader(&sdf->shader_loader);
	SetShaderVec2(sdf->shader_loader.shader, "body_size", (Vector2){GetScreenWidth(), GetScreenHeight()});

	sdf->texture = LoadTexture(PATH_TO_TEXTURE_64_WHITE);
}

static void DrawSDF(SDFObject *sdf)
{
	InitShaderMode(&sdf->shader_loader);
	DrawTextureQuad(sdf->texture, Vector2One(), Vector2Zero(), (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, WHITE);
	FinishShaderMode(&sdf->shader_loader);
}