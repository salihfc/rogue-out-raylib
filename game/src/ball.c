#include "raylib.h"
#include "utils.c"

typedef
struct Ball
{
    Vector2 position;
    Vector2 velocity;
    float radius;

} Ball;


static
void InitBall(Ball* ball, Vector2 position, Vector2 velocity, float radius)
{
    ball->position = position;
    ball->velocity = velocity;
    ball->radius = radius;
}


static
void DrawBall(Ball* ball)
{
    DrawCircleGradient(
        ball->position.x, ball->position.y,
        ball->radius + 0.0,
        BLACK, YELLOW
    );
}


static
void MoveBall(Ball* ball, float delta)
{
    ball->position = VectorSum(
        ball->position, 
        VectorScaled(ball->velocity, delta)
    );

    if (ball->position.x < 0 || SCREEN_BOUNDARY.width < ball->position.x) 
        ball->velocity.x *= -1;

    if (ball->position.y < 0 || SCREEN_BOUNDARY.height < ball->position.y) 
        ball->velocity.y *= -1;
}