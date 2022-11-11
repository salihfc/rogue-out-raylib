#include "raylib.h"

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
