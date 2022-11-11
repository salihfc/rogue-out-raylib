#include "raylib.h"

static const int BRICK_WIDTH = 100;
static const int BRICK_HEIGHT = 30;

typedef
struct Brick
{
    int remainingHp;
    Vector2 position;

} Brick;


static
void DrawBrick(Brick* brick)
{
    DrawRectangleV(brick->position, (Vector2) {BRICK_WIDTH, BRICK_HEIGHT}, Fade(RED, 0.7));
}