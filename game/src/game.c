#include "utils.c"
#include "player.c"
#include "board.c"
#include "ball.c"

typedef 
struct Game
{
    int frame;
    Player player;
    Board board;
    Ball ball;

} Game;


static
void InitGame(Game* game)
{
    game->frame = 0;
    ResetPlayerPosition(&(game->player));
    InitBoard(&(game->board), (Vector2) {200, 100});
    InitBall(&(game->ball), (Vector2) {200, 100}, (Vector2) {200, 80}, 10.0);
}

static
void UpdateGame(Game* game, float delta)
{
    MovePlayer(&(game->player), delta);
    MoveBall(&(game->ball), delta);
}


static
void DrawGame(Game* game, float delta)
{
    // DRAW FRAME
    DrawInt(game->frame / 60, (Vector2){10, 50}, 32, RED);

    // DRAW PLAYER
    Player* player = &(game->player);
    DrawPlayer(player);

    // DRAW BOARD
    Board* board = &(game->board);
    DrawBoard(board);

    // DRAW BALL
    Ball* ball = &(game->ball);
    DrawBall(ball);

    // DRAW DEBUG
    Vector2 mouse = GetMousePosition();
    DrawInt(mouse.x, (Vector2){10, 50 + 32 * 3}, 32, BLUE);
    DrawInt(mouse.y, (Vector2){100, 50 + 32 * 3}, 32, BLUE);
}