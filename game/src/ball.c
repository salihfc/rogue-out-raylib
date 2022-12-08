#include "raylib.h"
#include "utils/geo_utils.c"
#include "light/light.c"

static const float DEFAULT_BALL_LIGHT_INTENSITY = 1500.0;

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    float radius;

    ParticleEmitter particle_emitter;
    UniLight *light;
} Ball;

static void InitBall(Ball *ball, Vector2 position, Vector2 velocity, float radius)
{
    ball->position = position;
    ball->velocity = velocity;
    ball->radius = radius;
    ball->light->intensity = DEFAULT_BALL_LIGHT_INTENSITY;

    ball->particle_emitter =
        (ParticleEmitter){
            .base_color = YELLOW,
            .alpha = (Range){.min = 0.4, .max = 1.0},
            .lifetime = (Range){.min = 0.5, .max = 1},
            .size = (Range){.min = 2, .max = 4},
            .speed = (Range){.min = 0, .max = 0},

            .spread_angle = (Range){0, 2 * PI},
            .spread_distribution = UNIFORM,
            .offset_from_parent = (Vector2){0, 0},

            .emission_per_second = 100.0,
            .particle_per_emission = 1,
            .particle_type = CIRCLE,
        };

    InitParticleEmitter(&ball->particle_emitter);
}

static void DrawBall(Ball *ball)
{
    // Main body
    DrawCircleGradient(
        ball->position.x, ball->position.y,
        ball->radius + 0.0,
        BLACK, YELLOW);

    // Particles
    DrawParticleEmitter(&ball->particle_emitter);
}

static void AddSpeedToBall(Ball *ball, float speed)
{
    ball->velocity = VectorScaled(
        VectorNormalized(ball->velocity),
        VectorLen(ball->velocity) + speed);
}

static void MoveBall(Ball *ball, float delta)
{
    ball->position = VectorSum(
        ball->position,
        VectorScaled(ball->velocity, delta));

    if (ball->position.x < 0 || GetScreenRect().width < ball->position.x)
        ball->velocity.x *= -1;

    if (ball->position.y < 0 || GetScreenRect().height < ball->position.y)
        ball->velocity.y *= -1;
}

static void TickBall(Ball *ball, float delta)
{
    MoveBall(ball, delta);

    TickParticleEmitter(&ball->particle_emitter, delta, ball->position);
    if (ball->light)
    {
        ball->light->position = ball->position;
    }
}