#include "raylib.h"
#include "geo_utils.c"
#include "utils.c"

typedef
struct
{
    Vector2 position;
    Vector2 velocity;
    float radius;

    ParticleEmitter particle_emitter;
} Ball;


static
void InitBall(Ball* ball, Vector2 position, Vector2 velocity, float radius)
{
    ball->position = position;
    ball->velocity = velocity;
    ball->radius = radius;

    ball->particle_emitter = 
        (ParticleEmitter) {
        .base_color = YELLOW,
        .alpha = (Range) {.min = 0.4, .max = 1.0},
        .lifetime = (Range) {.min = 0.5, .max = 1},
        .size = (Range) {.min = 2, .max = 4},
        .speed = (Range) {.min = 0, .max = 10},

        .spread_angle = (Range) { 0, 2 * PI },
        .spread_distribution = UNIFORM,
        .offset_from_parent = (Vector2) {0, 0},

        .emission_per_second = 100.0,
        .particle_per_emission = 1,
    };
    
    InitParticleEmitter(&ball->particle_emitter);
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


    // Particles

    DrawParticleEmitter(&ball->particle_emitter);
}


static
void AddSpeedToBall(Ball* ball, float speed)
{
    ball->velocity = VectorScaled(
        VectorNormalized(ball->velocity),
        VectorLen(ball->velocity) + speed
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


static
void TickBall(Ball* ball, float delta)
{
    MoveBall(ball, delta);

    TickParticleEmitter(&ball->particle_emitter, delta, ball->position);
}