#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "particle.c"

#ifndef __PARTICLE_MANAGER
#define __PARTICLE_MANAGER

static const float EXTEND_RATIO = 0.9f;
static const int PARTICLE_MANAGER_STARTING_CAPACITY = 128;
static const int MAX_PARTICLE_MANAGER_CAPACITY = 16 * PARTICLE_MANAGER_STARTING_CAPACITY;

typedef
struct
{
    int particle_count;

    int capacity;
    Particle* particles;

    int* active_particles;
    int* active_buffer_0;
    int* active_buffer_1;


    int* free_particles;
    int free_count;

} ParticleManager;


static void MarkParticleFree(ParticleManager* particle_manager, int idx);
static int _GetFreeIdx(ParticleManager* particle_manager);
static int* _GetCurrentActiveBuffer(ParticleManager* particle_manager);
static int* _GetNextActiveBuffer(ParticleManager* particle_manager);
static bool _ReallocRequired(ParticleManager* particle_manager, int addition);
static void _IncreaseCapacity(ParticleManager* particle_manager);



static
void InitParticleManager(ParticleManager* particle_manager, int starting_capacity)
{
    particle_manager->capacity = starting_capacity;

    particle_manager->particles = MemAlloc(particle_manager->capacity * sizeof(Particle));
    particle_manager->free_particles = MemAlloc(particle_manager->capacity * sizeof(int));
    
    particle_manager->active_buffer_0 = MemAlloc(particle_manager->capacity * sizeof(int));
    particle_manager->active_buffer_1 = MemAlloc(particle_manager->capacity * sizeof(int));
    particle_manager->active_particles = particle_manager->active_buffer_0;

    int* it = particle_manager->free_particles;
    for (int i = 0; i < particle_manager->capacity; i++)
    {
        *it = i;
        it++;
    }
    particle_manager->free_count = particle_manager->capacity;
    particle_manager->particle_count = 0;
}


static
void TickParticleManager(ParticleManager* particle_manager, float delta)
{
    int* it = particle_manager->active_particles;
    int* next_buffer = _GetNextActiveBuffer(particle_manager);
    int* buffer_it = next_buffer;
    int ct = 0;

    for (int i = 0; i < particle_manager->particle_count; i++)
    {
        Particle* particle = &particle_manager->particles[*it];
        TickParticle(particle, delta);

        if (IsParticleActive(particle))
        {
            *buffer_it = *it;
            buffer_it++;
            ct++;
        }
        else // mark free
        {
            MarkParticleFree(particle_manager, *it);
        }

        it++;
    }

    particle_manager->active_particles = next_buffer;
    particle_manager->particle_count = ct;
}


static
void DrawParticleManager(ParticleManager* particle_manager)
{
    // TraceLog(3, "DRAWING PARTICLES");

    int* it = particle_manager->active_particles;
    for (int i = 0; i < particle_manager->particle_count; i++)
    {
        Particle* particle = &particle_manager->particles[*it];
        DrawParticle(particle);
        it++;
    }
}



static
void AddParticle(ParticleManager* particle_manager, Particle particle)
{
    if (_ReallocRequired(particle_manager, 1))
    {
        _IncreaseCapacity(particle_manager);
        printf("[ParticleManager] Realloc Required ==> NEW CAP: [%d]\n", particle_manager->capacity);
        fflush(stdout);
    }

    int* current_active = _GetCurrentActiveBuffer(particle_manager);
    int free_idx = _GetFreeIdx(particle_manager);

    if (free_idx == -1) // Skip generating particle
        return;

    // Set alive
    particle.life = particle.lifetime;

    // copy into array
    *(particle_manager->particles + free_idx) = particle;
    *(current_active + particle_manager-> particle_count) = free_idx;

    particle_manager->free_count--;
    particle_manager->particle_count++;

}


static
void AddParticleGroup(ParticleManager* particle_manager, Particle particle_defaults, int group_size, Vector2 direction)
{
    if (particle_defaults.immunity_duration == 0)
        particle_defaults.immunity_duration = PARTICLE_IMMUNITY;

    // Randomize some properties
    // Velocity only for now
    for (int idx = 0; idx < group_size; idx++)
    {
        particle_defaults.velocity = VectorScaled(
            VectorRotated(direction, GetRandomFloat(-PI/4, PI/4)),
            GetRandomValue(10, 200)
        );
        
        AddParticle(particle_manager, particle_defaults);
    }
}


static
void MarkParticleFree(ParticleManager* particle_manager, int idx)
{
    assert(idx < particle_manager->capacity);

    *(particle_manager->free_particles + particle_manager->free_count) = idx;
    particle_manager->free_count++;
}


static
int _GetFreeIdx(ParticleManager* particle_manager)
{
    if (particle_manager->free_count <= 0)
        return -1;

    return *(particle_manager->free_particles + particle_manager->free_count - 1);
}

static
int* _GetCurrentActiveBuffer(ParticleManager* particle_manager)
{
    return (particle_manager->active_particles == particle_manager->active_buffer_0) ? particle_manager->active_buffer_0 : particle_manager->active_buffer_1;
}


static
int* _GetNextActiveBuffer(ParticleManager* particle_manager)
{
    return (particle_manager->active_particles == particle_manager->active_buffer_0) ? particle_manager->active_buffer_1 : particle_manager->active_buffer_0;
}


static
bool _ReallocRequired(ParticleManager* particle_manager, int addition)
{
    return (float)(particle_manager->particle_count + addition) >= (particle_manager->capacity * EXTEND_RATIO);
}


static
void _IncreaseCapacity(ParticleManager* particle_manager)
{
    if (particle_manager->capacity >= MAX_PARTICLE_MANAGER_CAPACITY) return;

    particle_manager->capacity *= 2;
    particle_manager->particles = MemRealloc(particle_manager->particles, particle_manager->capacity * sizeof(Particle));
    particle_manager->free_particles = MemRealloc(particle_manager->free_particles, particle_manager->capacity * sizeof(int));

    for (int i = particle_manager->capacity / 2; i < particle_manager->capacity; i++)
        MarkParticleFree(particle_manager, i);
    

    bool zero_active = (particle_manager->active_particles == particle_manager->active_buffer_0);
    particle_manager->active_buffer_0 = MemRealloc(particle_manager->active_buffer_0, particle_manager->capacity * sizeof(int));
    particle_manager->active_buffer_1 = MemRealloc(particle_manager->active_buffer_1, particle_manager->capacity * sizeof(int));

    if (zero_active)
        particle_manager->active_particles = particle_manager->active_buffer_0;
    else
        particle_manager->active_particles = particle_manager->active_buffer_1;
}


#endif