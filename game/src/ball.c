#include "raylib.h"
#include "geo_utils.c"
#include "utils.c"

typedef
struct
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
    // Main body
    DrawCircleGradient(
        ball->position.x, ball->position.y,
        ball->radius + 0.0,
        BLACK, YELLOW
    );

    // Tail 0
    Vector2 prev_pos = VectorDif(
        ball->position,
        VectorScaled(
            VectorNormalized(ball->velocity),
            10.0
        )
    );

    DrawCircleGradient(
        prev_pos.x, prev_pos.y,
        ball->radius - 2.0,
        Fade(BLACK, 0.7), Fade(YELLOW, 0.7)
    );

    // Tail 1
    prev_pos = VectorDif(
        ball->position,
        VectorScaled(
            VectorNormalized(ball->velocity),
            20.0
        )
    );

    DrawCircleGradient(
        prev_pos.x, prev_pos.y,
        ball->radius - 4.0,
        Fade(BLACK, 0.7), Fade(YELLOW, 0.7)
    );
}


static
void MoveBall(Ball* ball, float delta)
{
    ball->position = VectorSum(
        ball->position, 
        VectorScaled(ball->velocity, delta)
    );

    if (ball->position.x < 0 || GetScreenRect().width < ball->position.x) 
        ball->velocity.x *= -1;

    if (ball->position.y < 0 || GetScreenRect().height < ball->position.y) 
        ball->velocity.y *= -1;
}