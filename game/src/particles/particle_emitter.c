#include "raylib.h"
#include "../my_range.c"
#include "../utils/geo_utils.c"
#include "particle_manager.c"

#ifndef __PARTICLE_EMITTER
#define __PARTICLE_EMITTER

typedef struct ParticleEmitter
{
	float emission_per_second;
	float particle_per_emission;
	float emit_frequency;

	MyRange lifetime;
	MyRange speed;
	MyRange size;
	MyRange alpha;

	Color base_color;

	MyRange spread_angle;
	Distribution spread_distribution;

	ParticleManager particle_manager;
	Vector2 offset_from_parent;

	float timer;

	ParticleType particle_type;
} ParticleEmitter;

static void InitParticleEmitter(ParticleEmitter *emitter)
{
	InitParticleManager(&emitter->particle_manager, 128);
	emitter->emit_frequency = 1.0f / emitter->emission_per_second;
	emitter->timer = 0.0;

	printf("Init Particle emitter with f:[%f]\n", emitter->emit_frequency);
}

static void _EmitterEmit(ParticleEmitter *emitter, Vector2 parent_position)
{
	for (int particle_idx = 0; particle_idx < emitter->particle_per_emission; particle_idx++)
	{
		AddParticle(&emitter->particle_manager,
								(Particle){
										.type = emitter->particle_type,
										.lifetime = GetRandomInRange(emitter->lifetime, UNIFORM_DISTRIBUTION),
										.base_size = GetRandomInRange(emitter->size, UNIFORM_DISTRIBUTION),
										.color = Fade(emitter->base_color, GetRandomInRange(emitter->alpha, UNIFORM_DISTRIBUTION)),

										.velocity = VectorScaled(AngleToVector(GetRandomInRange(emitter->spread_angle, emitter->spread_distribution)), GetRandomInRange(emitter->speed, UNIFORM_DISTRIBUTION)),
										.position = VectorSum(parent_position, emitter->offset_from_parent),

										.damping = 0.7,
										.immunity_duration = 1.0,
										.size_easing = EASE_CIRCLE4,
								});
	}
}

static void TickParticleEmitter(ParticleEmitter *emitter, float delta, Vector2 parent_position)
{
	// Movement Handled by manager
	TickParticleManager(&emitter->particle_manager, delta);
	// printf("particle manager tick\n");

	// tick
	emitter->timer += delta;

	// printf("emitter timer: [%f] >=? [%f]\n", emitter->timer, emitter->emit_frequency);

	if (emitter->timer >= emitter->emit_frequency)
	{
		emitter->timer = 0;
		// Emit
		_EmitterEmit(emitter, parent_position);
	}
}

static void DrawParticleEmitter(ParticleEmitter *emitter)
{
	DrawParticleManager(&emitter->particle_manager);
}

#endif