#include "raylib.h"
#include "geo_utils.c"

typedef
enum ParticleType{
    NONE = -1,
    CIRCLE = 0,
    SQUARE = 1,
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
}


static
void DrawParticle(Particle* particle)
{
    switch (particle->type)
    {
    case NONE:
        break;
    case CIRCLE:
        DrawCircleV(particle->position, particle->size, particle->color);
        break;
    case SQUARE:
        DrawRectangleV(particle->position, (Vector2){particle->size, particle->size}, particle->color);
        break;
    
    default:
        break;
    }
}