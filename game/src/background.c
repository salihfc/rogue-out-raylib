#include "raylib.h"
#include "raymath.h"
#include "shader_loader.c"

static const char *BACKGROUND_FRAGMENT_FILE = "game/shaders/background.fs";

typedef struct Background
{
	Texture2D texture;
	Vector2 size;
	Color tint;

	ShaderLoader shader_loader;
} Background;

static void InitBackground(Background *background)
{
	background->shader_loader = (ShaderLoader){
			.use_frag = true,
			.fragment_filename = BACKGROUND_FRAGMENT_FILE};

	InitShaderLoader(&background->shader_loader);

	SetShaderVec2(background->shader_loader.shader, "body_size", background->size);
}

static void SetBackgroundTint(Background *background, Color color)
{
	SetShaderVec4(background->shader_loader.shader, "color_hint", ColorNormalize(color));
}

static Rectangle GetRectBackground(Background *background)
{
	return (Rectangle){
			.x = 0,
			.y = 0,
			.width = background->size.x,
			.height = background->size.y};
}

static void DrawBackground(Background *background)
{
	InitShaderMode(&background->shader_loader);
	{
		DrawTextureQuad(background->texture, Vector2One(), Vector2Zero(), GetRectBackground(background), WHITE);
	}
	FinishShaderMode(&background->shader_loader);
}