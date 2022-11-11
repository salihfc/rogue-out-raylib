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
void HandleCollisions(Game* game, float delta)
{
    // Ball 

}


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

    HandleCollisions(game, delta);
}



static
void DrawGame(Game* game, float delta)
{
    // DRAW PLAYER
    Player* player = &(game->player);
    DrawPlayer(player);

    // DRAW BOARD
    Board* board = &(game->board);
    DrawBoard(board);

    // DRAW BALL
    Ball* ball = &(game->ball);
    DrawBall(ball);
}