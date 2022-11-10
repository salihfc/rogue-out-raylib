#include "utils.c"
#include "player.c"

typedef 
struct Game
{
    int frame;
    Player player;


} Game;


static
void InitGame(Game* game)
{
    game->frame = 0;
    ResetPlayerPosition(&(game->player));
}


static
void DrawGame(Game* game, float delta)
{
    const char* p_text = IntToConstChar(game->frame / 60);
    DrawText(p_text, 10, 50, 32, RED);
    MemFree((void *)p_text);

    Player* player = &(game->player);

    DrawPlayer(player);
}