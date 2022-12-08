#include "raylib.h"
#include "utils/geo_utils.c"
#include "shader_loader.c"

static const int BRICK_WIDTH = 70;
static const int BRICK_HEIGHT = 30;
static const float BRICK_BORDER_SIZE = 2.0;
static const Color BRICK_BORDER_COLOR = (Color){100, 100, 100, 0};

typedef struct
{
    int remainingHp;
    Vector2 position;

    ShaderLoader shader_loader;
    Texture2D texture;
} Brick;

static Rectangle GetBrickRect(Brick *brick)
{
    return (Rectangle){
        brick->position.x, brick->position.y,
        BRICK_WIDTH, BRICK_HEIGHT};
}

static Vector2 GetBrickCenter(Brick *brick)
{
    return VectorSum(
        brick->position,
        VectorScaled(
            (Vector2){BRICK_WIDTH, BRICK_HEIGHT},
            0.5));
}

static Color GetBrickColorFromHp(int hp)
{
    const int COLOR_COUNT = 4;
    assert(hp < COLOR_COUNT);
    return (Color[]){
        Fade(WHITE, 0.4),
        Fade(YELLOW, 0.3),
        Fade(ORANGE, 0.2),
        Fade(RED, 0.1),
    }[hp];
}

static void InitBrick(Brick *brick)
{
    brick->shader_loader = (ShaderLoader){
        .use_frag = true,
        .fragment_filename = "game/shaders/brick.fs"};

    brick->texture = LoadTexture("game/resources/textures/64x64_white.png");

    InitShaderLoader(&brick->shader_loader);
    SetShaderVec4(brick->shader_loader.shader, "color_hint", ColorNormalize(GetBrickColorFromHp(brick->remainingHp)));
    SetShaderVec4(brick->shader_loader.shader, "border_color", ColorNormalize(BRICK_BORDER_COLOR));
    SetShaderFloat((brick->shader_loader.shader), "border_thickness", BRICK_BORDER_SIZE);
    SetShaderVec2(brick->shader_loader.shader, "body_size", (Vector2){GetBrickRect(brick).width, GetBrickRect(brick).height});
    // SetShaderVec2(brick->shader_loader.shader, "gi_direction", (Vector2){0.0, -1.0});
}

static void DealDamageToBrick(Brick *brick, int damage)
{
    brick->remainingHp -= damage;
    SetShaderVec4(brick->shader_loader.shader, "color_hint", ColorNormalize(GetBrickColorFromHp(brick->remainingHp)));
}

static bool IsBrickDestroyed(Brick *brick)
{
    return brick->remainingHp <= 0;
}

static void DrawBrick(Brick *brick)
{
    if (IsBrickDestroyed(brick))
        return;

    InitShaderMode(&brick->shader_loader);
    {
        Rectangle rect = GetBrickRect(brick);
        // DrawRectangleRec(rect, color);
        Color color = GetBrickColorFromHp(brick->remainingHp);
        DrawTextureQuad(brick->texture, Vector2One(), Vector2Zero(), rect, color);
    }
    FinishShaderMode(&brick->shader_loader);
}
