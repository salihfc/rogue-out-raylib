#include "raylib.h"

#define SOUND_ENABLED false

#ifndef __SOUND
#define __SOUND

typedef enum {
	SOUND_NONE = -1,
	BALL_BRICK_COLLISION = 0,

	SFX_COUNT,
} SFX;

static const char SOUND_FILES[(int)SFX_COUNT][64] = {
	"resources/sfx/zzox/zzox-fx current-move-1.wav", 
};

typedef
struct SoundManager {
	Sound* sounds;
} SoundManager;


static
void InitSoundManager(SoundManager* sound_manager)
{
	sound_manager->sounds = MemAlloc((int) SFX_COUNT * sizeof(Sound));

	for (int i = 0; i < (int) SFX_COUNT; i++)
		*(sound_manager->sounds + i) = LoadSound(SOUND_FILES[i]);
}


static
Sound GetSound(SoundManager* sound_manager, SFX sfx)
{
	return sound_manager->sounds[sfx];
}

static
void PlaySFX(SoundManager* sound_manager, SFX sfx)
{
	if (!SOUND_ENABLED) return;

	assert(sfx < (int)SFX_COUNT);
	PlaySound(GetSound(sound_manager, sfx));
}

static
void PlaySFXRandomize(SoundManager* sound_manager, SFX sfx)
{
	if (!SOUND_ENABLED) return;

	Sound sound = GetSound(sound_manager, sfx);

	float volume = GetRandomFloat(0.7, 0.9);
	float pitch = GetRandomFloat(0.7, 1.0);

    SetSoundVolume(sound, volume);                 // Set volume for a sound (1.0 is max level)
    SetSoundPitch(sound, pitch);                   // Set pitch for a sound (1.0 is base level)
	PlaySound(sound);
}

#endif