#include "raylib.h"
#include "utils.c"

#ifndef _PLAYER
#define _PLAYER

static const Vector2 DEFAULT_POS = (Vector2) {568.0, 700.0};
static const Vector2 DEFAULT_SIZE = (Vector2) {200.0, 40.0};
static const float DEFAULT_SPEED = 100.0;

typedef 
struct Player
{
    Vector2 position;
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
}


static
void MovePlayer(Player* player, Vector2 movement)
{
    player->position.x += movement.x * DEFAULT_SPEED;
    player->position.y += movement.y * DEFAULT_SPEED;
} 


static 
void DrawPlayer(Player* player)
{
    if (player->texture.id)
    {
        DrawTextureEx(player->texture, player->position, 0.0, 1.0, WHITE);
    }
    else
    {
        DrawRectangleV(player->position, DEFAULT_SIZE, GREEN);
    }
}


#endif