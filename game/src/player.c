#include "raylib.h"
#include "2d_object.c"
#include "particles/particle_emitter.c"
#include "shader_loader.c"

#ifndef _PLAYER
#define _PLAYER

static const Vector2 DEFAULT_POS = (Vector2){568.0, 900.0};
static const Vector2 DEFAULT_SIZE = (Vector2){200.0, 40.0};
static const float DEFAULT_SPEED = 300.0;
static const float DEFAULT_DAMP = 0.9;
static const float PLAYER_BORDER_THICKNESS = 5.0f;
static const Color PLAYER_BORDER_COLOR = BLUE;

typedef struct Player
{
    Object2D body;
    Texture2D texture;
    Color tint;

    ParticleEmitter left_emitter;
    ParticleEmitter right_emitter;

    ShaderLoader shader_loader;
} Player;

static void InitPlayer(Player *player)
{
    player->shader_loader = (ShaderLoader){
        .use_frag = true,
        .fragment_filename = "game/shaders/base.fs"};

    player->tint = GREEN;
    InitShaderLoader(&(player->shader_loader));
    SetShaderVec4(player->shader_loader.shader, "color_hint", ColorNormalize(player->tint));
    SetShaderVec4(player->shader_loader.shader, "border_color", ColorNormalize(PLAYER_BORDER_COLOR));
    SetShaderFloat((player->shader_loader.shader), "border_thickness", PLAYER_BORDER_THICKNESS);

    player->left_emitter =
        (ParticleEmitter){
            .base_color = DARKGREEN,
            .alpha = (MyRange){.min = 0.4, .max = 1.0},
            .lifetime = (MyRange){.min = 1.0, .max = 1.5},
            .size = (MyRange){.min = 4, .max = 6},
            .speed = (MyRange){.min = 20, .max = 30},

            .spread_angle = (MyRange){PI, 3 * PI / 2},
            .spread_distribution = UNIFORM_DISTRIBUTION,
            .offset_from_parent = (Vector2){0, player->body.size.y},

            .emission_per_second = 10.0,
            .particle_per_emission = 4,
            .particle_type = SQUARE,
        };

    InitParticleEmitter(&player->left_emitter);

    player->right_emitter =
        (ParticleEmitter){
            .base_color = DARKGREEN,
            .alpha = (MyRange){.min = 0.4, .max = 1.0},
            .lifetime = (MyRange){.min = 1.0, .max = 1.5},
            .size = (MyRange){.min = 4, .max = 6},
            .speed = (MyRange){.min = 20, .max = 30},

            .spread_angle = (MyRange){-PI / 2.0, 0},
            .spread_distribution = UNIFORM_DISTRIBUTION,
            .offset_from_parent = (Vector2){player->body.size.x, player->body.size.y},

            .emission_per_second = 10.0,
            .particle_per_emission = 4,
            .particle_type = SQUARE,
        };

    InitParticleEmitter(&player->right_emitter);
}

static void LoadPlayerTexture(Player *player, const char *texture_filename)
{
    player->texture = LoadTexture(texture_filename);
}

static void SetPlayerSize(Player *player, Vector2 size)
{
    player->body.size = size;
    SetShaderVec2(player->shader_loader.shader, "body_size", size);
}

static void ResetPlayerPosition(Player *player)
{
    player->body.position = DEFAULT_POS;
    player->body.velocity = (Vector2){0, 0};
    player->body.damping_factor = DEFAULT_DAMP;
    SetPlayerSize(player, DEFAULT_SIZE);
}

static void ApplyForceToPlayer(Player *player, Vector2 force)
{
    ApplyForceToObject(&(player->body), VectorScaled(force, DEFAULT_SPEED));
}

static void MovePlayer(Player *player, float delta)
{
    Moveobject(&(player->body), delta);
}

static void TickPlayer(Player *player, float delta)
{
    MovePlayer(player, delta);

    TickParticleEmitter(&player->left_emitter, delta, player->body.position);
    TickParticleEmitter(&player->right_emitter, delta, player->body.position);
}

static Rectangle GetPlayerRect(Player *player)
{
    return (Rectangle){
        player->body.position.x, player->body.position.y,
        player->body.size.x, player->body.size.y};
}

static void DrawPlayer(Player *player)
{
    DrawParticleEmitter(&player->left_emitter);
    DrawParticleEmitter(&player->right_emitter);

#if SHADERS_ENABLED
    InitShaderMode(&player->shader_loader);
    DrawTextureQuad(player->texture, Vector2One(), Vector2Zero(), GetPlayerRect(player), player->tint);
    FinishShaderMode(&player->shader_loader);
#else
    DrawTextureQuad(player->texture, Vector2One(), Vector2Zero(), GetPlayerRect(player), player->tint);
#endif
}

#endif