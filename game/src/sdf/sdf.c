#include "raylib.h"
#include "raymath.h"
#include "../shader_loader.c"

// static const int MAX_RECTS = 128;
// static const int MAX_CIRCLES = 128;

static const char *SHADER_PARAM_CIRCLE_COUNT = "circle_count";
static const char *SHADER_PARAM_CIRCLES = "circles";

static const char *SHADER_PARAM_RECT_COUNT = "rect_count";
static const char *SHADER_PARAM_RECTS = "rects";

typedef struct SDFObject
{
	ShaderLoader shader_loader;
	Texture2D texture;

	int rect_count;
	Vector4 rects[128];

	int circle_count;
	Vector3 circles[128];

} SDFObject;

static void AddCircle(SDFObject *sdf, Vector3 circle_data)
{
	sdf->circles[sdf->circle_count] = circle_data;
	sdf->circle_count++;

	SetShaderInt(sdf->shader_loader.shader, SHADER_PARAM_CIRCLE_COUNT, sdf->circle_count);
	SetShaderVec3Array(sdf->shader_loader.shader, SHADER_PARAM_CIRCLES, (float *)sdf->circles, sdf->circle_count);
}

static void AddRect(SDFObject *sdf, Vector4 rect_data)
{
	sdf->rects[sdf->rect_count] = rect_data;
	sdf->rect_count++;

	SetShaderInt(sdf->shader_loader.shader, SHADER_PARAM_RECT_COUNT, sdf->rect_count);
	SetShaderVec4Array(sdf->shader_loader.shader, SHADER_PARAM_RECTS, (float *)sdf->rects, sdf->rect_count);
}

static void InitSDF(SDFObject *sdf)
{
	sdf->shader_loader = (ShaderLoader){
			.use_frag = true,
			.fragment_filename = "game/shaders/sdf.fs"};

	InitShaderLoader(&sdf->shader_loader);
	SetShaderVec2(sdf->shader_loader.shader, "body_size", (Vector2){GetScreenWidth(), GetScreenHeight()});

	sdf->texture = LoadTexture(PATH_TO_TEXTURE_64_WHITE);

	// AddCircle(sdf, (Vector3){400.0, 400.0, 60.0});
	// AddRect(sdf, (Vector4){400.0, 400.0, 40.0, 80.0});
}

static void DrawSDF(SDFObject *sdf)
{
	InitShaderMode(&sdf->shader_loader);
	DrawTextureQuad(sdf->texture, Vector2One(), Vector2Zero(), (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, WHITE);
	FinishShaderMode(&sdf->shader_loader);
}