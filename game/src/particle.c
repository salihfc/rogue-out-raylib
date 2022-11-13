#include <assert.h>

#include "raylib.h"
#include "geo_utils.c"
#include "easings.c"

typedef
enum ParticleType{
    NONE = -1,
    CIRCLE = 0,
    SQUARE = 1,

    COUNT
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
    Color final_color = Fade(particle->color, t);

    switch (particle->type)
    {
    case NONE:
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