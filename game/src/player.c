#include "raylib.h"
#include "2d_object.c"
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
    Object2D body;
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
void DrawPlayer(Player* player)
{
    DrawRectangleV(player->body.position, player->body.size, GREEN);
 
    // Debug
    DrawText(IntToConstChar((int)(player->body.position.x)), 10, 80, 30, DARKGREEN);
    DrawText(IntToConstChar((int)(player->body.position.y)), 10, 80 + 30, 30, DARKGREEN);

    DrawText(IntToConstChar((int)(player->body.size.x)), 100, 80, 30, DARKGREEN);
    DrawText(IntToConstChar((int)(player->body.size.y)), 100, 80 + 30, 30, DARKGREEN);
}


#endif