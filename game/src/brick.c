#include "raylib.h"
#include "geo_utils.c"

static const int BRICK_WIDTH = 30;
static const int BRICK_HEIGHT = 10;

typedef
struct Brick
{
    int remainingHp;
    Vector2 position;

} Brick;

static
Rectangle GetBrickRect(Brick* brick)
{
    return (Rectangle) {
        brick->position.x, brick->position.y,
        BRICK_WIDTH, BRICK_HEIGHT
    };
}


static
Vector2 GetBrickCenter(Brick* brick)
{
    return VectorSum(
        brick->position,
        VectorScaled(
            (Vector2){BRICK_WIDTH, BRICK_HEIGHT},
            0.5
        )
    );
}


static
bool IsBrickDestroyed(Brick* brick)
{
    return brick->remainingHp <= 0;
}


static
void DrawBrick(Brick* brick)
{
    if (!IsBrickDestroyed(brick))
    {
        Color color = (Color[]){
            Fade(WHITE, 0.0),
            Fade(YELLOW, 0.7),
            Fade(ORANGE, 0.9),
            Fade(RED, 1.0),
        }[brick->remainingHp];

        DrawRectangleRec(GetBrickRect(brick), color);
    }
}
