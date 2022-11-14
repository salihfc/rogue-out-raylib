#include <assert.h>

#include "raylib.h"
#include "geo_utils.c"
#include "easings.c"

static const float PARTICLE_IMMUNITY = 0.2;

typedef enum {
    PARTICLE_NONE = -1,
    CIRCLE = 0,
    SQUARE = 1,

    PARTICLE_TYPE_COUNT
} ParticleType;

typedef
struct Particle
{
    ParticleType type;

    Vector2 position;
    Vector2 velocity;

    float size;

    float lifetime;
    float life;
    float life_t;

    float damping;

    Color color;

} Particle;


static
bool IsParticleActive(Particle* particle)
{
    return particle->life > 0.0;
}

static
bool IsParticleImmune(Particle* particle)
{
    return (particle->lifetime - particle->life) < PARTICLE_IMMUNITY;
}


static
void MoveParticle(Particle* particle, float delta)
{
    particle->life -= delta;
    particle->position = VectorSum(particle->position, VectorScaled(particle->velocity, delta));

    assert(particle->lifetime);
    particle->life_t = particle->life / particle->lifetime;

    float damp = 1.0 - (1.0 - particle->damping) * delta;
    particle->velocity = VectorScaled(particle->velocity, damp);
}


static
void DrawParticle(Particle* particle)
{
    float t = ease2_smoothstep(particle->life_t);
    Color final_color = particle->color;
    final_color.a *= t;

    switch (particle->type)
    {
    case PARTICLE_NONE:
        break;
    case CIRCLE:
        DrawCircleV(particle->position, particle->size, final_color);
        break;
    case SQUARE:
        DrawRectangleV(particle->position, (Vector2){particle->size, particle->size}, final_color);
        break;
    
    default:
        break;
    }
}