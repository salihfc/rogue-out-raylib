#include "raylib.h"
#include "utils.c"

#ifndef _PLAYER
#define _PLAYER

static const Vector2 DEFAULT_POS = (Vector2) {568.0, 900.0};
static const Vector2 DEFAULT_SIZE = (Vector2) {200.0, 40.0};
static const float DEFAULT_SPEED = 300.0;
static const float DEFAULT_DAMP = 0.9;

typedef 
struct Player
{
    Vector2 position;
    Vector2 velocity;

    Texture2D texture;

} Player;


static 
void LoadPlayerTexture(Player* player, const char* texture_filename)
{
    player->texture = LoadTexture(texture_filename);
}


static 
void ResetPlayerPosition(Player* player)
{
    player->position = DEFAULT_POS;
    player->velocity = (Vector2) {0, 0};
}


static
void ApplyForceToPlayer(Player* player, Vector2 force)
{
    player->velocity = VectorSum(player->velocity, force);
}


static
void MovePlayer(Player* player, float delta)
{
    // Horizontal movement
    player->position.x += player->velocity.x * DEFAULT_SPEED;
    player->position.x = Clamp(player->position.x, 0, 1356 - DEFAULT_SIZE.x);

    // Vertical movement
    player->position.y += player->velocity.y * DEFAULT_SPEED;
    player->position.y = Clamp(player->position.y, 0, 900 - DEFAULT_SIZE.y);

    // Damping
    player->velocity.x *= DEFAULT_DAMP;
    player->velocity.y *= DEFAULT_DAMP;
}


static 
void DrawPlayer(Player* player)
{
    DrawRectangleV(player->position, DEFAULT_SIZE, GREEN);
 
    // Debug
    DrawText(IntToConstChar((int)(player->position.x)), 10, 80, 30, DARKGREEN);
    DrawText(IntToConstChar((int)(player->position.y)), 10, 80 + 30, 30, DARKGREEN);
}


#endif