#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "particle.c"

static const float EXTEND_RATIO = 0.9f;
static const int PARTICLE_GENERATOR_STARTING_CAPACITY = 128;
static const int MAX_PARTICLE_GENERATOR_CAPACITY = 16 * PARTICLE_GENERATOR_STARTING_CAPACITY;

typedef
struct ParticleGenerator
{
    int particle_count;

    int capacity;
    Particle* particles;

    int* active_particles;
    int* active_buffer_0;
    int* active_buffer_1;


    int* free_particles;
    int free_count;

} ParticleGenerator;


static void MarkParticleFree(ParticleGenerator* particle_generator, int idx);
static int _GetFreeIdx(ParticleGenerator* particle_generator);
static int* _GetCurrentActiveBuffer(ParticleGenerator* particle_generator);
static int* _GetNextActiveBuffer(ParticleGenerator* particle_generator);
static bool _ReallocRequired(ParticleGenerator* particle_generator, int addition);
static void _IncreaseCapacity(ParticleGenerator* particle_generator);



static
void InitParticleGenerator(ParticleGenerator* particle_generator, int starting_capacity)
{
    particle_generator->capacity = starting_capacity;

    particle_generator->particles = MemAlloc(particle_generator->capacity * sizeof(Particle));
    particle_generator->free_particles = MemAlloc(particle_generator->capacity * sizeof(int));
    
    particle_generator->active_buffer_0 = MemAlloc(particle_generator->capacity * sizeof(int));
    particle_generator->active_buffer_1 = MemAlloc(particle_generator->capacity * sizeof(int));
    particle_generator->active_particles = particle_generator->active_buffer_0;

    int* it = particle_generator->free_particles;
    for (int i = 0; i < particle_generator->capacity; i++)
    {
        *it = i;
        it++;
    }
    particle_generator->free_count = particle_generator->capacity;
    particle_generator->particle_count = 0;
}


static
void TickParticleGenerator(ParticleGenerator* particle_generator, float delta)
{
    int* it = particle_generator->active_particles;
    int* next_buffer = _GetNextActiveBuffer(particle_generator);
    int* buffer_it = next_buffer;
    int ct = 0;

    for (int i = 0; i < particle_generator->particle_count; i++)
    {
        Particle* particle = &particle_generator->particles[*it];
        MoveParticle(particle, delta);

        if (IsParticleActive(particle))
        {
            *buffer_it = *it;
            buffer_it++;
            ct++;
        }
        else // mark free
        {
            MarkParticleFree(particle_generator, *it);
        }

        it++;
    }

    particle_generator->active_particles = next_buffer;
    particle_generator->particle_count = ct;
}


static
void DrawParticleGenerator(ParticleGenerator* particle_generator)
{
    // TraceLog(3, "DRAWING PARTICLES");

    int* it = particle_generator->active_particles;
    for (int i = 0; i < particle_generator->particle_count; i++)
    {
        Particle* particle = &particle_generator->particles[*it];
        DrawParticle(particle);
        it++;
    }
}



static
void AddParticle(ParticleGenerator* particle_generator, Particle particle)
{
    if (_ReallocRequired(particle_generator, 1))
    {
        _IncreaseCapacity(particle_generator);
        TraceLog(LOG_INFO, "[ParticleGenerator] Realloc Required");
        printf("NEW CAP: [%d]", particle_generator->capacity);
    }

    int* current_active = _GetCurrentActiveBuffer(particle_generator);
    int free_idx = _GetFreeIdx(particle_generator);

    if (free_idx == -1) // Skip generating particle
        return;

    // Set alive
    particle.life = particle.lifetime;

    // copy into array
    *(particle_generator->particles + free_idx) = particle;
    *(current_active + particle_generator-> particle_count) = free_idx;

    particle_generator->free_count--;
    particle_generator->particle_count++;

}


static
void AddParticleGroup(ParticleGenerator* particle_generator, Particle particle_defaults, int group_size, Vector2 direction)
{
    // Randomize some properties
    // Velocity only for now
    for (int idx = 0; idx < group_size; idx++)
    {
        particle_defaults.velocity = VectorScaled(
            VectorRotated(direction, GetRandomFloat(-PI/4, PI/4)),
            GetRandomValue(10, 200)
        );
        
        AddParticle(particle_generator, particle_defaults);
    }
}


static
void MarkParticleFree(ParticleGenerator* particle_generator, int idx)
{
    assert(idx < particle_generator->capacity);

    *(particle_generator->free_particles + particle_generator->free_count) = idx;
    particle_generator->free_count++;
}


static
int _GetFreeIdx(ParticleGenerator* particle_generator)
{
    if (particle_generator->free_count <= 0)
        return -1;

    return *(particle_generator->free_particles + particle_generator->free_count - 1);
}

static
int* _GetCurrentActiveBuffer(ParticleGenerator* particle_generator)
{
    return (particle_generator->active_particles == particle_generator->active_buffer_0) ? particle_generator->active_buffer_0 : particle_generator->active_buffer_1;
}


static
int* _GetNextActiveBuffer(ParticleGenerator* particle_generator)
{
    return (particle_generator->active_particles == particle_generator->active_buffer_0) ? particle_generator->active_buffer_1 : particle_generator->active_buffer_0;
}


static
bool _ReallocRequired(ParticleGenerator* particle_generator, int addition)
{
    return (float)(particle_generator->particle_count + addition) >= (particle_generator->capacity * EXTEND_RATIO);
}


static
void _IncreaseCapacity(ParticleGenerator* particle_generator)
{
    if (particle_generator->capacity >= MAX_PARTICLE_GENERATOR_CAPACITY) return;

    particle_generator->capacity *= 2;
    particle_generator->particles = MemRealloc(particle_generator->particles, particle_generator->capacity * sizeof(Particle));
    particle_generator->free_particles = MemRealloc(particle_generator->free_particles, particle_generator->capacity * sizeof(int));

    for (int i = particle_generator->capacity / 2; i < particle_generator->capacity; i++)
        MarkParticleFree(particle_generator, i);
    

    bool zero_active = (particle_generator->active_particles == particle_generator->active_buffer_0);
    particle_generator->active_buffer_0 = MemRealloc(particle_generator->active_buffer_0, particle_generator->capacity * sizeof(int));
    particle_generator->active_buffer_1 = MemRealloc(particle_generator->active_buffer_1, particle_generator->capacity * sizeof(int));

    if (zero_active)
        particle_generator->active_particles = particle_generator->active_buffer_0;
    else
        particle_generator->active_particles = particle_generator->active_buffer_1;
}

