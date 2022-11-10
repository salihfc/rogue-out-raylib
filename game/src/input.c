#include "raylib.h"


static
bool IsMoveLeftDown()
{
    return IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
}

static
bool IsMoveRightDown()
{
    return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
}