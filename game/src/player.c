#include "raylib.h"
#include "2d_object.c"
#include "utils.c"
#include "geo_utils.c"
#include "particle_emitter.c"

#ifndef _PLAYER
#define _PLAYER

static const Vector2 DEFAULT_POS = (Vector2) {568.0, 900.0};
static const Vector2 DEFAULT_SIZE = (Vector2) {200.0, 40.0};
static const float DEFAULT_SPEED = 300.0;
static const float DEFAULT_DAMP = 0.9;

typedef 
struct Player
{
    Object2D body;
    Texture2D texture;
    Color tint;

    ParticleEmitter left_emitter;
    ParticleEmitter right_emitter;
} Player;

static
void InitPlayer(Player* player)
{
    player->left_emitter =
        (ParticleEmitter) {
            .base_color = DARKGREEN,
            .alpha = (Range) {.min = 0.4, .max = 1.0},
            .lifetime = (Range) {.min = 1.0, .max = 1.5},
            .size = (Range) {.min = 4, .max = 6},
            .speed = (Range) {.min = 20, .max = 30},

            .spread_angle = (Range) { PI, 3 * PI / 2 },
            .spread_distribution = UNIFORM,
            .offset_from_parent = (Vector2) {0, player->body.size.y},

            .emission_per_second = 10.0,
            .particle_per_emission = 4,
            .particle_type = SQUARE,
        };
 
    InitParticleEmitter(&player->left_emitter);

    player->right_emitter =
        (ParticleEmitter) {
            .base_color = DARKGREEN,
            .alpha = (Range) {.min = 0.4, .max = 1.0},
            .lifetime = (Range) {.min = 1.0, .max = 1.5},
            .size = (Range) {.min = 4, .max = 6},
            .speed = (Range) {.min = 20, .max = 30},

            .spread_angle = (Range) { -PI / 2.0, 0 },
            .spread_distribution = UNIFORM,
            .offset_from_parent = (Vector2) {player->body.size.x, player->body.size.y},

            .emission_per_second = 10.0,
            .particle_per_emission = 4,
            .particle_type = SQUARE,
        };
 
    InitParticleEmitter(&player->right_emitter);
}


static 
void LoadPlayerTexture(Player* player, const char* texture_filename)
{
    player->texture = LoadTexture(texture_filename);
}


static 
void ResetPlayerPosition(Player* player)
{
    player->body.position = DEFAULT_POS;
    player->body.velocity = (Vector2) {0, 0};
    player->body.damping_factor = DEFAULT_DAMP;
    player->body.size = DEFAULT_SIZE;
}


static
void ApplyForceToPlayer(Player* player, Vector2 force)
{
    ApplyForceToObject(&(player->body), VectorScaled(force, DEFAULT_SPEED));
}


static
void MovePlayer(Player* player, float delta)
{
    Moveobject(&(player->body), delta);
}


static
void TickPlayer(Player* player, float delta)
{
    MovePlayer(player, delta);

    TickParticleEmitter(&player->left_emitter, delta, player->body.position);
    TickParticleEmitter(&player->right_emitter, delta, player->body.position);
}


static 
void DrawPlayer(Player* player)
{
    DrawParticleEmitter(&player->left_emitter);
    DrawParticleEmitter(&player->right_emitter);

    DrawRectangleV(player->body.position, player->body.size, player->tint);
}


static
Rectangle GetPlayerRect(Player* player)
{
    return (Rectangle) {
        player->body.position.x, player->body.position.y,
        player->body.size.x, player->body.size.y
    };
}

#endif