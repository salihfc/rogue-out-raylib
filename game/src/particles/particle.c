#include <assert.h>

#include "raylib.h"
#include "../utils/geo_utils.c"
#include "../animation/easings.c"

#ifndef __PARTICLE
#define __PARTICLE

static const float PARTICLE_IMMUNITY = 0.2;

typedef enum
{
    PARTICLE_NONE = -1,
    CIRCLE = 0,
    SQUARE = 1,

    PARTICLE_TYPE_COUNT
} ParticleType;

typedef struct Particle
{
    ParticleType type;

    Vector2 position;
    Vector2 velocity;

    float base_size;
    float size;

    float lifetime;
    float life;
    float life_t;

    float damping;

    Color color;

    float immunity_duration;

    // animation
    EASING size_easing;

} Particle;

static bool IsParticleActive(Particle *particle)
{
    return particle->life > 0.0;
}

static bool IsParticleImmune(Particle *particle)
{
    return (particle->lifetime - particle->life) < particle->immunity_duration;
}

static void MoveParticle(Particle *particle, float delta)
{
    particle->position = VectorSum(particle->position, VectorScaled(particle->velocity, delta));

    float damp = 1.0 - (1.0 - particle->damping) * delta;
    particle->velocity = VectorScaled(particle->velocity, damp);
}

static void TickParticle(Particle *particle, float delta)
{
    MoveParticle(particle, delta);

    particle->life -= delta;
    assert(particle->lifetime);
    // anim_t calculation
    particle->life_t = particle->life / particle->lifetime;
    particle->size = (1 - apply_easing(particle->size_easing, particle->life_t)) * particle->base_size;
}

static void DrawParticle(Particle *particle)
{
    if (particle->size < 0.01)
        return;

    float t = ease2_smoothstep(particle->life_t);
    Color final_color = particle->color;
    final_color.a *= t;

    float final_size = particle->size * 4.0;

    Vector2 final_position = particle->position;

    switch (particle->type)
    {
    case PARTICLE_NONE:
        break;
    case CIRCLE:
        DrawCircleV(final_position, final_size, final_color);
        break;
    case SQUARE:
        DrawRectangleV(final_position, (Vector2){final_size, final_size}, final_color);
        break;

    default:
        break;
    }
}

#endif