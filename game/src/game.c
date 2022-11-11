#include "utils.c"
#include "player.c"
#include "board.c"

typedef 
struct Game
{
    int frame;
    Player player;
    Board board;

} Game;


static
void InitGame(Game* game)
{
    game->frame = 0;
    ResetPlayerPosition(&(game->player));
    InitBoard(&(game->board), (Vector2) {200, 100});
}


static
void DrawGame(Game* game, float delta)
{
    DrawInt(game->frame / 60, (Vector2){10, 50}, 32, RED);
    Player* player = &(game->player);
    DrawPlayer(player);
    Board* board = &(game->board);
    DrawBoard(board);

    Vector2 mouse = GetMousePosition();
    DrawInt(mouse.x, (Vector2){10, 50 + 32 * 3}, 32, BLUE);
    DrawInt(mouse.y, (Vector2){100, 50 + 32 * 3}, 32, BLUE);
}